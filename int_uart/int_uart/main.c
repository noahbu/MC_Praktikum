/*
 * int_uart.c
 *
 * Created: 03.04.2024 09:58:16
 * Author : delfi
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "uart.h"


int main(void)
{
    // Pin der roten Ampel A als Ausgang konfigurieren

    //DDRD |= (1 << PD6); // PD6 als Ausgang konfigurieren
    DDRD |= (1 << PD5); // PD5 als Ausgang konfigurieren

    // variable für UART empfangenes Zeichen
    uint8_t rec;

    // UART initialisieren und interrupts lokal aktivieren
    uart_init();

    // globale Interrupts aktivieren
    sei();

    while (1) 
    {

        //toggle rote LED an Ampel A
        PORTD ^= (1 << PD5);

        //Lese empfangenes Zeichen und verschicke es wieder
        while(uart_chinbuf()){
            //empfangenes Zeichen lesen und in Variable rec speichern
            rec = uart_getch();
            //empfangenes Zeichen wieder verschicken
            uart_putch(rec);
        }
		 
		 // sende einen Zeilenvorschub und neue Zeile
		 uart_puts((uint8_t *)"\n"); //Zwei zeichen --> String

        _delay_ms(250);

    }
}

