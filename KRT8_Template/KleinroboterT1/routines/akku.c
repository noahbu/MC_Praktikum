#include "akku.h"

/* Initialisierungsroutine für das Auslesen der Akkuspannung */
void akku_init()
{
	/* ADC auf Kannal ADC3 setzen und lediglich ADCH Register verwenden */
	ADMUX |= ((1<<MUX0) | (1<<MUX1) | (1<<ADLAR));
	
	/* ADC aktivieren und Prescale auf 128 setzten */
	ADCSRA |= ((1<<ADEN) | (1<<ADPS0) | (1<<ADPS1) | (1<<ADPS2));
	
	/* Erste ADC-Wandlung starten */
	ADCSRA |= (1<<ADSC);
	while (ADCSRA & (1<<ADSC));
}

/* Routine zum  Abfragen der aktuellen Akkuspannung */
float akku_voltage()
{
	/* ADC auf Kannal ADC3 setzen und lediglich ADCH Register verwenden */
	ADMUX |= ((1<<MUX0) | (1<<MUX1) | (1<<ADLAR));
	
	/* ADC-Wandlung starten */
	ADCSRA |= (1<<ADSC);
	while (ADCSRA & (1<<ADSC));
	uint8_t voltage8 = ADCH;
	
	/* Wandlungsergebnis vom ADC3 in einen Spannungswert umrechnen */
	return ((float)voltage8/255.0*SPANNUNGSTEILER*VREF);
}

/* Routine zur Überpüfung, ob die Akkuspannung nicht unterhalb des kritischen Wertes liegt */
void akku_check()
{
	/* Wenn der Akku leer ist wird in eine Endlosschleife gesprungen,
	 so dass das User-Programm nicht gestartet wird */
		
	/* Abfragen der aktuellen Akkuspannung */
	float voltage = akku_voltage();
	
	/* Überprüfen ob die aktuelle Akkuspannung innerhalb des zulässigen Bereichs ist */
	if (voltage < V_LOW)
	{
		sei();	// Sei() einschalten, um eine Meldung per UART versenden zu können
		
		char send[30] = ""; // Char-Buffer von 30 Zeichen für Meldung
		
		/* Endlosschleife mit Warnung --> Akku leer --> gelbe LED Blinkt*/
		while (1)
		{
			/* Warnmeldung verschicken */
			sprintf(send, "Akku leer! U_Akku = %.1fV \n\r", voltage);
			uart_puts((uint8_t*)send);
			
			/* gelbe LED an PA1 blinken lassen */
			PORTA ^= (1<<PA1);
			_delay_ms(1000);
			PORTA ^= (1<<PA1);
			_delay_ms(1000);
		}
	}
}	