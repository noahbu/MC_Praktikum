/* Einbinden der ben�tigten Standardbibliotheken */
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
uint8_t activeController = TESTING;

/* Hauptfunktion */
int main(void)
{	
	/* LEDs als Ausg�nge setzen */
	DDRA |= (1<<LED_GRUEN);
	DDRA |= (1<<LED_GELB);
	DDRA |= (1<<LED_ROT);
	DDRB |= (1<<LED_GRUENB);
	
	/* Initialisierung starten */
	// LED auf rot --> Kleinroboter nicht anfassen w�hren Initialisierung
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
	
	/* Initialisierung abschlie�en */
	// Akku Notabschaltung
	akku_check();
	// Interrupts einschalten
	sei();
	// �ber UART mitteilen, dass die Initialisierung abgeschlossen ist
	uart_puts((uint8_t*)"FINISHED INITIALIZATION\r\n");
	// Rote LED aus --> Kleinroboter darf wieder bewegt werden
	PORTA &= ~(1<<LED_ROT);
	
	/* Programstart */
	PORTA |= (1<<LED_GELB);
	// Warten bis der Taster gedr�ckt wird
	while((PINB & (1<<PB2)));
	_delay_ms(1000);
	PORTA &= ~(1<<LED_GELB);
	PORTA |= (1<<LED_GRUEN);
		
	/* Hauptprogramm */
	while(1)
    {		
		/* Regler ausf�hren wenn das Sampleflag gesetzt wurde */
		if(sampleFlag)
		{					
			// Sampleflag zur�cksetzen
			sampleFlag = 0;
				
			if(activeController == MANUELLE_STEUERUNG) {
				/* Manual Control Mode */
				motor_manualCtrl();
			} else if(activeController == BALANCIERREGLER) {
				/* Balancierregler */
				reglerBalancieren_regelung();
			} else if(activeController == TRAJEKTORIENREGLER) {
				/* Vorsteuerung und Regelung
					Bitte die zum jeweils ben�tigten Funktionen einkommentieren */
				float u1_B, u2_B, x1_B, x2_B, x3_B;
				reglerTrajektorienfolge_beobachter(&u1_B, &u2_B, &x1_B, &x2_B, &x3_B);
				// reglerTrajektorienfolge_steuerung();
				reglerTrajektorienfolge_regelung(u1_B, u2_B, x1_B, x2_B, x3_B);
			} else if(activeController == TESTING) {
				/* CODE START */
				
				motor_manualCtrl();

			
				// print encoder values in UART
				int32_t countsL = qdec_getCounts(FLAG_SPI_QDEC_L);
				int32_t countsR = qdec_getCounts(FLAG_SPI_QDEC_R);

				char buffer[128];
				// Assuming you want each number to occupy exactly 6 characters (+/- included), adjust the 6 as needed
				snprintf(buffer, sizeof(buffer), "L: %6ld, R: %6ld \r\n", countsL, countsR);
	
				// print acc values in UART
				//int16_t accData[3]; // Array to hold accelerometer data
    			//acc_getData(accData); // Fetch the accelerometer data

				//snprintf(buffer, sizeof(buffer), "Acc X: %d, Y: %d, Z: %d\r\n", accData[0], accData[1], accData[2]);
				uart_puts((uint8_t*)buffer);

				

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
	// Toggle der roten LED um sehen zu k�nnen ob der Interrupt ausgef�hrt wird
	PORTB ^= (1<<LED_GRUENB);
}
