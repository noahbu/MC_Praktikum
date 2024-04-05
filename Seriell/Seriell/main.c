/*
 * Seriell.c
 *
 * Created: 02.04.2024 11:27:28
 * Author : delfi
 */ 

#include <avr/io.h>
#include <util/delay.h>

#define BAUD 57600
#include <util/setbaud.h>

int main(void)
{
	
	//Baudrate für UART einstellen
	UBRRH = UBRRH_VALUE;
	UBRRL = UBRRL_VALUE;
	
	# if USE_2X
	UCSRA |= (1<<U2X);
	# else
	UCSRA &= ~(1<<U2X);
	#endif
	
	
	// Format einstellen: 8data, 1stop, no parity (8N1-Modus)
	// DIeser ist im default schon eingstellt, deswegen keine Änderungen
	
	//enable transmitter and receiver
	UCSRB |= (1<<TXEN)|(1<<RXEN);
	
	// variablen erstellen
	
	uint8_t rec; //Variable für ein über UART empfangenes zeichen
	uint8_t pause = 1; //Variable für die Pause zwischen zwei signalen
	uint8_t counter = 0; // Zähler für versendetes zeichen

	
    while (1) 
    {
		//abfragen, ob sich im EIngangsbuffer ein ungelesenes Zeichen befindet

		while(UCSRA & (1<<RXC)){
			//lese empfangenes zeichen aus buffer
			rec = UDR;
			
			//empfangenes Zeichen auswerten
			if (rec == '+' && pause >1)
			{
				pause--; //schneller senden
			}
			else if (rec == '-' && pause <254)
			{
				pause++; // langsamer senden
			}
			
		}
	
		
		//Zäherwert senden
		UDR = ('0' + counter);
		//Zählweert erhöhen und durch Modulo begrenzen auf 0-9
		
		counter = (counter+1)%10;
		
		//warten bis zeichen verschickt sind
		while(!(UCSRA & (1<<TXC)));
		
		// Flag händisch auf Null setzten
		
		UCSRA |=(1<<TXC);
		
		//warten gemäß definierter Pause
		for(uint8_t i=0; i<pause;i++)
		{
			_delay_ms(100);
		}
		//_delay_ms(pause*100);
    }
}

