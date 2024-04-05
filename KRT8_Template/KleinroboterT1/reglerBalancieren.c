#include "reglerBalancieren.h"

/* CODE START */
/* CODE END */


/* Tiefpassfilter 2. Ordnung welcher vom Komplement�rfilter verwendet wird */
float reglerBalancieren_PT2(float x)
{	// PT2-Glied mit Eckfrequenz wie �ber COEFF_x bestimmt
	/* CODE START */
	/* CODE END */
}

/* Komplement�rfilter zur Fusion der zwei Messungen
   OUTPUTS: - alphaCompFilter:	Gesch�tzter Winkel
			- alpha_dot:		Gemessene Drehrate */
void reglerBalancieren_komplementaerFilter(float* alphaCompFilter, float* alpha_dot) 
{
	/* CODE START */
	/* CODE END */
}

/* Winkel-Regler (innere Schleife der Kaskade)
	INPUTS: - alpha:		Gesch�tzer Winkel
			- alpha_dot:	Gemessene Winkelgeschwindigkeit
			- alpha_soll:	Sollwinkel */
void reglerBalancieren_winkelRegler(float alpha, float alpha_dot, float alpha_soll) 
{	
	/* CODE START */
	/* CODE END */	
}

/* Geschwindigkeitsregler (�u�ere Schleife der Kaskade)
	INPUTS: - vRad:			Gemessene Radgeschwindigkeit
			- alpha:		Gesch�tzter Winkel
			- alpha_dot:	Gemessene Winkelgeschwindigkeit */
void reglerBalancieren_geschwindigkeitsRegler(float vRad, float alpha, float alpha_dot) 
{		
	// Zust�nde f�r Umfalllogik des Geschwindigkeitsreglers
	static int8_t istUmgefallen = 0; // Umfallzustand des Roboters
	static int32_t reactivateCount = 0;
	/* CODE START */
	/* CODE END */	
	/* Logik zur Aktivierung, Deaktivierung und Reaktivierung des Reglers nach dem Umfallen */
	if (!istUmgefallen)	{ // Roboter aktuell nicht umgefallen		
		/* Teste ob der Roboter inzwischen umgefallen ist */
		if (fabs(alpha) < ALPHA_ABS_DEACTIVATE) { // Roboter ist NICHT umgefallen
			/* Hier muss die �u�ere Kaskade (der PID Geschwindigkeitsregler) implementiert werden */
			/* CODE START */
			/* CODE END */		
		} else { // Der Roboter ist gerade umgefallen, Flag setzen
			istUmgefallen = 1;
			
			// �ber UART diesen Zustand �bermitteln
			char send[50];
			sprintf(send, "Roboter ist umgefallen\r\n");
			uart_puts((uint8_t*)send);
			
			// Motoren ausschalten
			motor_setVel(0.0f, 0.0f);			
		}
	} else { /* Roboter ist immer noch umgefallen, �berwachung ob der Roboter wieder aufgerichtet wurde
				--> Roboter muss mindestens 2 Sekunden lang in aufrechter Position sein */
		/* Wurde der Roboter wieder aufgerichtet? */
		if (fabs(alpha) < ALPHA_ABS_ACTIVATE)
		{ // Roboter wurde wieder aufgerichtet			
			if(reactivateCount == 0) { // Information der Reaktivierung per UART verschicken
				char send[30];;
				sprintf(send, "Reactivation in 2 secs:\r\n");
				uart_puts((uint8_t*)send);
				reactivateCount++;
			} else if(reactivateCount < 2.0f/T_SAMPLE) { // Warten auf Reaktivierung (2 Sekunden lang)
				reactivateCount++;
			} else { // Reaktivierung des Reglers				
				istUmgefallen = 0;
				reactivateCount = 0;
				
				// Regler neu initialisieren
				reglerBalancieren_init();				
			}			
		}
	}
}

/* Initialiserung des Reglers und der verwendet Filter */
void reglerBalancieren_init(void) 
{	
	/* CODE START */
	/* CODE END */
}

/* Ausf�hrung des Reglers zum Balancieren 
	- Sensordaten lesen 
	- Komplement�rfilter ausf�hren
	- Regler ausf�hren */
void reglerBalancieren_regelung(void) 
{
	/* CODE START */
	/* CODE END */
}

/* CODE START */
/* CODE END */
