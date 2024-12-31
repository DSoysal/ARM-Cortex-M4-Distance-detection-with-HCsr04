#include "TM4C123GH6PM.h"

void timer_pin_init(void);

void timer_pin_init(void)
{
    SYSCTL->RCGCTIMER |= (1<<3);     /* enable clock to Timer Block 3 */
    SYSCTL->RCGCGPIO |= (1<<1);    /* enable clock to PORTB */
    
    GPIOB->DIR &= ~(1<<2); /* set PB2 an input pin */
    GPIOB->DEN |= (1<<2); /* set PB2 a digital pin */
		GPIOB->AFSEL |= (1<<2); /* enable alternate function on PB2 */
		GPIOB->PCTL &= ~0x00000F00; /* configure PB2 as T3CCP0 pin */
		GPIOB->PCTL |= 0x00000700;
    
	  /* PA4 as a digital output signal to provide trigger signal */
	  SYSCTL->RCGCGPIO |= 1;      /* enable clock to PORTA */
	  GPIOA->DIR |=(1<<4);         /* set PA4 as a digial output pin */
	  GPIOA->DEN |=(1<<4);         /* make PA4 as digital pin */

    TIMER3->CTL &= ~1;          /* disable timer0A during setup */
    TIMER3->CFG = 4;            /* 16-bit timer mode */
    TIMER3->TAMR = 0x17;        /* up-count, edge-time, capture mode */
    TIMER3->CTL |=0x0C;        /* capture the rising edge */
    TIMER3->CTL |= (1<<0);           /* enable timer0A */
}