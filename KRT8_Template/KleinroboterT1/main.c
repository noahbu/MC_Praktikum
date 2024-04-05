/* Einbinden der benötigten Standardbibliotheken */
#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <util/twi.h>

/* Einbinden der bereitgestellten Module */
#include "routines/uart.h"
#include "routines/trajektorieVorgabe.h"
#include "routines/akku.h"
#include "routines/sensorenI2C.h"

/* Einbinden der im Laufe des Praktikums zu programmierenden Module */
#include "main.h"
#include "motor.h"
#include "sensorenSPI.h"
#include "reglerBalancieren.h"
#include "reglerTrajektorienfolge.h"

/* Sample Zeit Flag, TRUE Samplezeit */
volatile int8_t sampleFlag = 0;

/* Auswahl des Reglers welcher aktiv sein soll
	Zur Auswahl stehen:	- MANUELLE_STEUERUNG
						- BALANCIERREGLER
						- TRAJEKTORIENREGLER
						- TESTING */
uint8_t activeController = BALANCIERREGLER;

/* Hauptfunktion */
int main(void)
{	
	/* LEDs als Ausgänge setzen */
	DDRA |= (1<<LED_GRUEN);
	DDRA |= (1<<LED_GELB);
	DDRA |= (1<<LED_ROT);
	DDRB |= (1<<LED_GRUENB);
	
	/* Initialisierung starten */
	// LED auf rot --> Kleinroboter nicht anfassen währen Initialisierung
	PORTA |= (1<<LED_ROT);
	_delay_ms(1000);
	
	/*	Initialisiere Timer 0 zur Erzeugung der Samplezeit
		Samplezeit wird in als T_SAMPLE gesetzt */
	TIMSK |= (1<<OCIE0);									//enable output compare interrupt
	TCCR0 = (1<<WGM01)|(1<<CS02)|(1<<CS00);					// non PWM, normal operation OC0 disconnected, Clk = clk-IO / 1024
	OCR0 = (uint8_t)((float)F_CPU/1024 * T_SAMPLE);			// setze output compare register entsprechend
	
	/* Alle Module initialisieren */
	// UART/Bluetooth aktivieren und initialisieren
	uart_init();	
	// Akkuspanungsmessung initialisieren
	akku_init();	
	// Motoransteurung initialisieren
	motor_init();
	// SPI und die Sensoren am SPI Bus (Decoder und Accelerometer) initialisieren
	spi_init();
	qdec_init();
	acc_init();
	// I2C und die Sensoren am I2C Bus (Gyro) initialisieren
	gyro_init();
	// Regler initialisieren
	reglerTrajektorienfolge_init();
	reglerBalancieren_init();			
	
	/* Initialisierung abschließen */
	// Akku Notabschaltung
	akku_check();
	// Interrupts einschalten
	sei();
	// Über UART mitteilen, dass die Initialisierung abgeschlossen ist
	uart_puts((uint8_t*)"FINISHED INITIALIZATION\r\n");
	// Rote LED aus --> Kleinroboter darf wieder bewegt werden
	PORTA &= ~(1<<LED_ROT);
	
	/* Programstart */
	PORTA |= (1<<LED_GELB);
	// Warten bis der Taster gedrückt wird
	while((PINB & (1<<PB2)));
	_delay_ms(1000);
	PORTA &= ~(1<<LED_GELB);
	PORTA |= (1<<LED_GRUEN);
		
	/* Hauptprogramm */
	while(1)
    {		
		/* Regler ausführen wenn das Sampleflag gesetzt wurde */
		if(sampleFlag)
		{					
			// Sampleflag zurücksetzen
			sampleFlag = 0;
				
			if(activeController == MANUELLE_STEUERUNG) {
				/* Manual Control Mode */
				motor_manualCtrl();
			} else if(activeController == BALANCIERREGLER) {
				/* Balancierregler */
				reglerBalancieren_regelung();
			} else if(activeController == TRAJEKTORIENREGLER) {
				/* Vorsteuerung und Regelung
					Bitte die zum jeweils benötigten Funktionen einkommentieren */
				float u1_B, u2_B, x1_B, x2_B, x3_B;
				reglerTrajektorienfolge_beobachter(&u1_B, &u2_B, &x1_B, &x2_B, &x3_B);
				// reglerTrajektorienfolge_steuerung();
				reglerTrajektorienfolge_regelung(u1_B, u2_B, x1_B, x2_B, x3_B);
			} else if(activeController == TESTING) {
				/* CODE START */
				/* CODE END */
			}
			
			/* Warnung bei Nichteinhalten der Samplezeit */
			if(sampleFlag) {
				char buff[7] = { 'm', 'i', 's', 's', '\r', '\n', '\0' };
				uart_puts((uint8_t*)buff);				
			}
		}
	}
}

/* Timer-Interrupt */
ISR(TIMER0_COMP_vect) {
	// Setzen der Sample-Flag
	sampleFlag = 1;
	// Toggle der roten LED um sehen zu können ob der Interrupt ausgeführt wird
	PORTB ^= (1<<LED_GRUENB);
}
