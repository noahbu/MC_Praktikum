/*
 * Blinki_2.c
 *
 * Created: 02.04.2024 10:31:13
 * Author : delfi
 */ 

#include <avr/io.h>
#include <util/delay.h>


int main(void)
{
	
	// Pin zur ansteuerung der roten LED an Ampel A als AUsgang konfigurieren
	
	DDRD |= (1<<PIND5);
	DDRD |= (1<<PINC5);
	 
    while (1) 
    {
		//Pegel des Pins für die rote LED an Ampel A toggeln
		PORTD ^= (1<<PIND5);
		PORTC ^= (1<<PINC5);
		
		_delay_ms(500);
		
    }
}

