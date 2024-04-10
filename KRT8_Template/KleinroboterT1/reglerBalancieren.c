#include "reglerBalancieren.h"

/* CODE START */
float cumulativeAngleGyro = 0.0; // Cumulative angle in degrees
float alpha_Filter = 0.0; // Complementary filter angle

float z1; // PT2-Zustand 1
float z2; // PT2-Zustand 2
/* CODE END */


/* Tiefpassfilter 2. Ordnung welcher vom Komplement�rfilter verwendet wird */
float reglerBalancieren_PT2(float x)
{	// PT2-Glied mit Eckfrequenz wie �ber COEFF_x bestimmt
	/* CODE START */


	// PT2-Filterkoeffizienten
	float y_filt = 0.0f;

	//float u_filt = 0.0f;

	float filter_a2 = -1.9644605802;
	float filter_a3 = 0.9650811739;
	float filter_b1 = 1.0000000000;
	float filter_b2 = 2.0000000000;
	float filter_b3 = 1.0000000000;
	float filter_g = 0.0001551484;

	y_filt = filter_g*filter_b1*x+z1;
	z1 = filter_g*filter_b2*x + z2 + (-filter_a2*y_filt);
	z2 = filter_g*filter_b3*x + (-filter_a3*y_filt);

	return y_filt;

	/* CODE END */
}

/* Komplement�rfilter zur Fusion der zwei Messungen
   OUTPUTS: - alphaCompFilter:	Gesch�tzter Winkel
			- alpha_dot:		Gemessene Drehrate */
void reglerBalancieren_komplementaerFilter(float* alphaCompFilter, float* alpha_dot) {
    /* CODE START */

    float ALPHA_COMP = 1.0f; // Complementary filter factor
    float GYRO_SCALE_FACTOR = (1000.0f / 32768.0f); // Gyro scale factor for 1000 degrees/sec full scale

    // Read accelerometer and gyroscope data
    int16_t accData[3]; // Array to hold accelerometer data
    acc_getData(accData); // Fetch the accelerometer data

    int16_t gyroData[3]; // Array to hold gyroscope data
    gyro_getData(gyroData);

    // Calculate angle from accelerometer data

    //float alpha_acc = atan2f(accData[0], sqrt(accData[1] * accData[1] + accData[2] * accData[2]));
	float alpha_acc = atan2f(accData[2], accData[0]);
    alpha_acc *= (180.0f / M_PI); // Convert to degrees

    // Integrate gyroscope data to get angle
    // Assuming gyroData[1] corresponds to the axis you're integrating for
    cumulativeAngleGyro += (gyroData[1] * GYRO_SCALE_FACTOR) * T_SAMPLE;

    // Apply the complementary filter
    *alphaCompFilter = ALPHA_COMP * reglerBalancieren_PT2(alpha_acc - cumulativeAngleGyro)+cumulativeAngleGyro;

    // For debugging, send data over UART

    char buffer[150];
    snprintf(buffer, sizeof(buffer), "%3.5f,%3.5f,%3.5f\n\r", alpha_acc, cumulativeAngleGyro, *alphaCompFilter);
    uart_puts((uint8_t*)buffer);

    // Optional: Store the gyro rate if needed
    *alpha_dot = gyroData[1] * GYRO_SCALE_FACTOR;

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
