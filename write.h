#include "TM4C123GH6PM.h"

void UART0_init(void);
void UART0_Transmitter(unsigned char data);
void printstring(char *str);


void UART0_init(void)
{
    SYSCTL->RCGCUART |= 0x01;  /* Enable clock to UART0 */
    SYSCTL->RCGCGPIO |= 0x01;  /* Enable clock to PORTA for PA0/Rx and PA1/Tx */
    
    UART0->CTL = 0;            /* Disable UART0 module during configuration */
    UART0->IBRD = 104;         /* Integer part for 9600 baud rate */
    UART0->FBRD = 11;          /* Fractional part for 9600 baud rate */
    UART0->CC = 0;             /* Use system clock */
    UART0->LCRH = 0x60;        /* 8-bit data, no parity, 1 stop bit */
    UART0->CTL = 0x301;        /* Enable UART0 module, Rx and Tx */

    GPIOA->DEN = 0x03;         /* Enable digital functions for PA0 and PA1 */
    GPIOA->AFSEL = 0x03;       /* Enable alternate functions for PA0 and PA1 */
    GPIOA->PCTL = 0x11;        /* Configure PA0 and PA1 for UART */
}

void UART0_Transmitter(unsigned char data)
{
    while ((UART0->FR & (1 << 5)) != 0); /* Wait until Tx buffer is not full */
    UART0->DR = data;                    /* Transmit the data */
}

void printstring(char *str)
{
    while (*str)
    {
        UART0_Transmitter(*(str++));    /* Send characters one by one */
    }
}