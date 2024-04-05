/*
 * leddim.c
 *
 * Created: 03.04.2024 11:25:50
 * Author : delfi
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define INCR 500

#include "uart.h"

int main(void)
{

    //Pin für gelbe LED an Ampel B als Ausgang konfigurieren
    DDRB |= (1<<PB1);

    // Wert den wir empfangen wollen via UART
    uint8_t rec;

    // Timer 1 konfigurieren
    // Fast PWM Mode  (Top = ICR1), non inverting mode
    TCCR1A |= (1<<COM1A1) | (1<<WGM11);
    TCCR1B |= (1<<WGM13) | (1<<WGM12);

    // Prescaler 8
    TCCR1B |= (1<<CS11);

    // TOP wert setzen ( 0 <= ICR1 <= 65535)
    ICR1 = 30000;

    // wert für compare match einstellen (0 <= OCR1A <= ICR1)

    OCR1A = 15000;

    // UART initialisieren

    uart_init();

    // globale Interrupts aktivieren
    sei();
    
    while (1) 
    {

        // zeichen von UART lesen
        rec = uart_getch();

        //LED dimmen
        switch(rec)
        {
            //LED heller
            case '+':
                if(OCR1A + INCR < ICR1)
                {
                    OCR1A += INCR;
                }
                break;
            //LED dunkler
            case '-':
                if(OCR1A > INCR)
                {
                    OCR1A -= INCR;
                }
                break;
            //LED ganz hell
            case 'r':
                OCR1A = ICR1;
                break;
            //LED ganz dunkel
            case 'l':
                OCR1A = 1;
                break;
        }

        //etwas warten
        _delay_ms(100);


    }
}

