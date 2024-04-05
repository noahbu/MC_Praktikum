/*
 * int_tast.c
 *
 * Created: 03.04.2024 09:29:02
 * Author : delfi
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

int main(void)
{
	//Pins als Ausgange konfigurieren
	DDRD |= (1<<PD5);
	DDRD |= (1<<PD6);

    // externen Interrupt INTO auf steigende flanke konfigurieren
    MCUCR |= (1<<ISC00) | (1<<ISC01);
	
    // externen Interrupt lokal aktivieren
    GICR |= (1<<INT0);

    // globale Interrupts aktivieren
    sei();
	
    /* Replace with your application code */
    while (1) 
    {
        //gelbe LED an Ampel A anschalten
        PORTD |= (1<<PD6);
        _delay_ms(1000);
		
    }
}

ISR(INT0_vect){
    // togglen der roten LED an Ampel A
    PORTD ^= (1<<PD5);
}



