/*
 * ALU_test.c
 *
 * Created: 04.04.2024 09:51:23
 * Author : delfi
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "uart.h"


int main(void)
{
    uart_init();
    sei();

    //define pointer
    uint8_t *p;

    float f = 0.0002;
    p = (uint8_t *)&f;
	//u += 100;
	
   //uart_puts((uint8_t *)"\n\r");
    uart_putch(*(p+3));
	uart_putch(*(p+2));
	uart_putch(*(p+1));
	uart_putch(*p);
	
	//uart_puts((uint8_t *)"\n\r");
	
	uint8_t *q;
	
	float t = 0.0001;
	q = (uint8_t *)&t;
	//u += 100;
	
	uart_putch(*(q+3));
	uart_putch(*(q+2));
	uart_putch(*(q+1));
	uart_putch(*q);
	//uart_puts((uint8_t *)"\n\r");
    
    while (1) 
    {
    }
}

