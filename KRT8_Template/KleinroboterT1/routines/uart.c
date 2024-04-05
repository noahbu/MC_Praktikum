/*
* Baudrate für Kommunikation mit BTM222
*/
#define BAUD 38400

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/wdt.h>
#include <util/setbaud.h>
#include "uart.h"

const uint8_t reset_string[] = "BOOTLOADER";
uint8_t reset_pos;

void uart_init()
{
	uart_outbufpos=0;
	uart_outbufeval=0;

	uart_inbufpos=0;
	uart_inbufeval=0;

	reset_pos=0;
	
	UCSRC &= ~(1<<URSEL);
	UBRRH = UBRRH_VALUE;
	UBRRL = UBRRL_VALUE;
	
	#if USE_2X
		UCSRA |= (1 << U2X);
	#else
		UCSRA &= ~(1 << U2X);
	#endif
	   
	// 8data bits, 1stop bit, no parity
	// Set frame format: 8data, 1stop bit, no parity
	UCSRC |= (1<<URSEL);
	UCSRC = (1<<URSEL) | (1<<UCSZ1) | (1<<UCSZ0);
	
	// Enable transceiver
	UCSRB = (1<<TXEN)|(1<<RXEN)|(1<<RXCIE)|(1<<TXCIE);
}


void uart_putch(uint8_t c)
{
	// enqueue character to buffer
	uart_outbuf[uart_outbufpos++] = c;
	if (uart_outbufpos == UART_BUFSIZE)
		uart_outbufpos = 0;	
	uart_send();
}


void uart_puts (uint8_t* s)
{
	while (*s)
	{
		uart_putch((uint8_t)*s++);
	}
}

uint8_t uart_chinbuf()
{
	if (uart_inbufeval > uart_inbufpos)
		return (UART_BUFSIZE + uart_inbufpos - uart_inbufeval);
	else
		return uart_inbufpos - uart_inbufeval;
}

uint8_t uart_getch_wait()
{
	while (uart_inbufpos == uart_inbufeval);
	return uart_getch();
}

uint8_t uart_getch()
{
	// get one character from buffer
	if (uart_inbufpos == uart_inbufeval)
		return 0;
	
	uint8_t c = uart_inbuf[	uart_inbufeval++];
	
	if (uart_inbufeval == UART_BUFSIZE)
		uart_inbufeval = 0;

	return c;
}

void uart_send()
{
	// exit if transceiver is busy
	if (!(UCSRA & (1<<UDRE)))
		return;

	// send one character from buffer
	if (uart_outbufpos == uart_outbufeval)
		return;

	UDR = uart_outbuf[uart_outbufeval++];

	if (uart_outbufeval == UART_BUFSIZE)
		uart_outbufeval = 0;
}

void uart_receive()
{
	// receive character and put it to buffer
	uint8_t temp = UDR;
	
	uart_inbuf[uart_inbufpos++] = temp;
	
	if (uart_inbufpos == UART_BUFSIZE)
		uart_inbufpos = 0;

	// see whether reset command is being received
	if (*(reset_string+reset_pos) == temp)
	{
		reset_pos++;
		if (reset_pos == 10)
		{	// reset command entirely received
			uart_putch(99);
			//WDTCR |= (1<<WDE);
			wdt_enable(1);
			while(1);
		}
	}
	else
	{	
		// reset command not matched - reset pointer
		reset_pos=0;
	}
}

void uart_flush()
{
	uart_inbufpos=0;
	uart_inbufeval=0;
}


ISR(USART_TXC_vect)
{
	// character has been sent. see if there is more data in buffer
	uart_send();
}

ISR(USART_RXC_vect)
{
	// character received by uart. load it to buffer
	uart_receive();
}