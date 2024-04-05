/*
 * TasterLED.c
 *
 * Created: 02.04.2024 10:44:37
 * Author : delfi
 */ 

#include <avr/io.h>


int main(void)
{
	//Pin zur ansteuerung der roten LED an AMpel A als ausgang konfigurieren
	
	DDRD |=(1<<PIND5);
	DDRC |=(1<<5);
	
    /* Replace with your application code */
    while (1) 
    {
		//An Pin 2 liegt ein externer Pull-up Wiederstand an, der den Signalpegel auf high zieht
		//Wird der Joysticktaster gedrückt, so wird der Pegel an PIN2 auf low gezogen
		
		 
		
		//Pegel an Pin2 abfragen
		if((PIND & 1<<PD2) && (PIND & 1<<PD3)){
			PORTD &= ~(1<<PD5);
			PORTC &= ~(1<<5);
		}
		else{
			PORTD |= (1<<PD5);
			PORTC |= (1<<5);
		}
    }
}

