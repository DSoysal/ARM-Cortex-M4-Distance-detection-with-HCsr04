#include "TM4C123GH6PM.h"
#include "write.h"
#include "initializers.h"
#include <stdio.h>

/*Function prototype for Timer0A and UART module initialization */

void delay_ms(int time);

/* global variables to store and display distance in cm */
uint32_t time; /*stores pulse on time */
uint32_t distance; /* stores measured distance value */
char output[100];  /* string format of distance value */

/* main code to take distance measurement and send data to UART terminal */
int main(void)
{  
timer_pin_init();	
UART0_init();
	
	while(1)
	{
		int riseEdge, fallEdge;
	
	  /* Given 10us trigger pulse */
	  GPIOA->DATA &= ~(1<<4); /* make trigger  pin high */
	  delay_ms(10); /*10 seconds delay */
	  GPIOA->DATA |= (1<<4); /* make trigger  pin high */
	  delay_ms(10); /*10 seconds delay */
	  GPIOA->DATA &= ~(1<<4); /* make trigger  pin low */


    TIMER3->ICR = 4;            /* clear timer0A capture flag */
    while((TIMER3->RIS & 4) == 0) ;    /* wait till captured */
	  if(GPIOB->DATA & (1<<2)) /*check if rising edge occurs */
		{
    riseEdge = TIMER3->TAR;     /* save the timestamp */
		/* detect falling edge */
    TIMER3->ICR = 4;            /* clear timer0A capture flag */
    while((TIMER3->RIS & 4) == 0) ;    /* wait till captured */
    fallEdge = TIMER3->TAR;     /* save the timestamp */
		time = fallEdge-riseEdge;
			
		distance = (time * 10625)/10000000; /* convert pulse duration into distance */
    sprintf(output, "Distance = %d cm\r\n", distance); /*convert float type distance data into string */
    printstring(output); /*transmit data to computer */	
		}	
	}	
}


void delay_ms(int time)
{
    int i;
    SYSCTL->RCGCTIMER |= 2;     /* enable clock to Timer Block 1 */
    TIMER1->CTL = 0;            /* disable Timer before initialization */
    TIMER1->CFG = 0x04;         /* 16-bit option */ 
    TIMER1->TAMR = 0x02;        /* periodic mode and down-counter */
    TIMER1->TAILR = 16;  /* TimerA interval load value reg */
    TIMER1->ICR = 0x1;          /* clear the TimerA timeout flag */
    TIMER1->CTL |= 0x01;        /* enable Timer A after initialization */

    for(i = 0; i < time; i++)
    {
        while ((TIMER1->RIS & 0x1) == 0) ;      /* wait for TimerA timeout flag */
        TIMER1->ICR = 0x1;      /* clear the TimerA timeout flag */
    }
}
