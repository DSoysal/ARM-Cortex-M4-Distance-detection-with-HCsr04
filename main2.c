#include "TM4C123GH6PM.h"
#include "write.h"
#include "Pulse_init.h"
#include "timer_pin_init.h"
#include <stdio.h>

/*Function prototype for Timer0A and UART module initialization */

void delay(unsigned long counter);

/* global variables to store and display distance in cm */
uint32_t time; /*stores pulse on time */
uint32_t distance; /* stores measured distance value */
char output[100];  /* string format of distance value */

/* main code to take distance measurement and send data to UART terminal */
int main(void)
{  
timer_pin_init();	
UART0_init();
pulse_init();
	
	while(1)
	{
		
    int riseEdge;
    int	riseEdge2;
		int fallEdge;


    TIMER3->ICR = 4;            /* clear timer0A capture flag */
    while((TIMER3->RIS & 4) == 0);    /* wait till captured */
	  if(GPIOB->DATA & (1<<2)) /*check if rising edge occurs */
		{
    riseEdge = TIMER3->TAR;     /* save the timestamp */
		/* detect falling edge */
    TIMER3->ICR = 4;            /* clear timer0A capture flag */
    while((TIMER3->RIS & 4) == 0);    /* wait till captured */
    fallEdge = TIMER3->TAR;     /* save the timestamp */
		TIMER3->ICR = 4;	
		while((TIMER3->RIS & 4) == 0);    /* wait till captured */
    riseEdge2 = TIMER3->TAR;     /* save the timestamp */	
		time = fallEdge-riseEdge;	
		}

	
    uint32_t pulse_width = fallEdge - riseEdge;
    uint32_t period = riseEdge2 - riseEdge;
    float duty_cycle = ((float)pulse_width / (float)period) * 100.0;

    // Prepare the output message
    sprintf(output, "Pulse Width: %u ticks, Period: %u ticks, Duty Cycle: %.2f%%\r\n", pulse_width, period, duty_cycle);
 
    printstring(output);
    delay(2000);
	}	
}


void delay(unsigned long counter)
{
	unsigned long i = 0;
	
	for(i=0; i< counter*1000; i++);
}

