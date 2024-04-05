/*
 * uart.h
 *
 * Created: 13.02.2013 14:51:20
 *  Author: k.albert
 */ 

#ifndef UART_H_
#define UART_H_
// call uart_init() once at the beginning of your program
// to initialize the serial interface
void uart_init(void);

// put char into output-buffer
void uart_putch(uint8_t c);

// put string into output-buffer
void uart_puts (uint8_t* s);

// read byte from input-buffer
// returns 0 if no byte has been received
uint8_t uart_getch(void);

// read byte from input-buffer
// if nothing has been received, wait until there is data
uint8_t uart_getch_wait(void);

// starts transmission
// call this regularly to send your data!
void uart_send(void);

// receive byte and attach it to buffer
// (automatically called by ISR)
void uart_receive(void);

// return number of bytes in input-buffer
uint8_t uart_chinbuf(void);

// flush input-/output-buffers
void uart_flush(void);

#endif /* UART_H_ */