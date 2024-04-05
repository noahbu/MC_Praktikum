/*
 * ADC_ampel.c
 *
 * Created: 04.04.2024 11:06:04
 * Author : delfi
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "uart.h"

int main(void)
{
    //variable für 8 bit ADC Wert
    uint8_t adc_result;

    // Pin der roten Ampel A als Ausgang konfigurieren
    DDRD |= (1 << PD5); // PD5 als Ausgang konfigurieren
    DDRD |= (1 << PD6); // PD6 als Ausgang konfigurieren
    DDRD |= (1 << PD7); // PD7 als Ausgang konfigurieren


    // ADC initialisieren
    // ADC Prescaler auf 128 setzen
    ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
    // ADC aktivieren
    ADCSRA |= (1 << ADEN);
    // Referenzspannung auf AVCC setzen
    ADMUX |= (1 << REFS0) | (1 << ADLAR);
	
	//Auswahl von Kanal 7 (Fotodiode
	//ADMUX |= (1<<MUX0)|(1<<MUX1)|(1<<MUX2);
    ADMUX |= (1<<MUX2)|(1<<MUX1);
	
	//init uart
    uart_init();

    // globale Interrupts aktivieren
    sei();

    // erste Wandlung durchführen, damit ADC startklar ist (Warmup)
    ADCSRA |= (1 << ADSC);
    while(ADCSRA & (1 << ADSC));

    //wandlungsergebnis abfragen
    adc_result = ADCH;
    

    while (1) 
    {

        // ADC starten
        ADCSRA |= (1 << ADSC);
        // Warte bis ADC fertig ist
        while(ADCSRA & (1 << ADSC));

        // ADC Wert in Variable speichern
        adc_result = ADCH;
		
		//uart_putch((uint8_t *) adc_result);

        // Ampel schalten
		
        if(adc_result > 150){ //sehr hell
            PORTD |= (1 << PD7);
            PORTD &= ~(1 << PD6);
            PORTD &= ~(1 << PD5);
        }else if(adc_result > 110 ){ //mittel hell
            PORTD &= ~(1 << PD5);
            PORTD |= (1 << PD6);
            PORTD &= ~(1 << PD7);
        }else{ //dunkel
            PORTD &= ~(1 << PD7);
            PORTD &= ~(1 << PD6);
            PORTD |= (1 << PD5);
        }
		
		uart_putch(adc_result);

        // Warte 1 Sekunde
        //_delay_ms(1000);
    }
}

