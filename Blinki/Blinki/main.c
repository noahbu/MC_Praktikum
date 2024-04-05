/*
 * Binki.c
 *
 * Created: 02.04.2024 09:55:41
 * Author : delfi
 */ 

#include <avr/io.h>


int main(void)
{
	uint32_t a = 0;
	
	// Pin zu Ansteuerung der roten LED an Ampel A als AUsgang konfigurieren: LED12 ist mit PD5 verbunden
	*(volatile char *)(0x31) |= (1<<5);
	
    while (1) 
    {
		//Pegel des Pins der roten LED an Ampel A umschalten
		*(volatile char *)(0x32) ^= (1<<5);
		
		//LED mit 1 Hz blinken lasse, daher müssen wir etwas warten zwischen dem togglen
		
		// Warteschleife
		
		a=0;
		while(a < 8000000){
			//variable inkrementieren
			a++;
		}
    }
}

