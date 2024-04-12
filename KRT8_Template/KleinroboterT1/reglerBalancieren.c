#include "reglerBalancieren.h"

/* CODE START */
float cumulativeAngleGyro; // Cumulative angle in degrees
float alpha_Filter = 0.0; // Complementary filter angle

float z1; // PT2-Zustand 1
float z2; // PT2-Zustand 2

float integral_of_angle_error; // Integral of angle error
static float integral_of_velocity_error; // Initialize the static variable
static float previous_velocity_error; // To store previous error for derivative calculation

/* CODE END */


/* Tiefpassfilter 2. Ordnung welcher vom Komplement�rfilter verwendet wird */
float reglerBalancieren_PT2(float x)
{	// PT2-Glied mit Eckfrequenz wie �ber COEFF_x bestimmt
	/* CODE START */


	// PT2-Filterkoeffizienten
	float y_filt = 0.0f;

	//float u_filt = 0.0f;

	// f_cutoff = 0.3
	float filter_a2 = -1.9733442498;
	float filter_a3 = 0.9736948720;
	float filter_b1 = 1.0000000000;
	float filter_b2 = 2.0000000000;
	float filter_b3 = 1.0000000000;
	float filter_g = 0.0000876555;

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

    // For filter tuning, send data over UART
	/*
    char buffer[150];
    snprintf(buffer, sizeof(buffer), "%3.5f,%3.5f,%3.5f\n\r", alpha_acc, cumulativeAngleGyro, *alphaCompFilter);
    uart_puts((uint8_t*)buffer);
	*/
    //Store the gyro rate if needed
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

	// PID controller constants
	float Kp_alpha = -3.8843f;
	float Ki_alpha = -0.0144f;
	float Kd_alpha = -0.0021f;

	

	// Calculate the angle error
    float angle_error = alpha_soll - alpha;

	//without integral windup prevention
	integral_of_angle_error += angle_error * T_SAMPLE;

	//integral_of_angle_error += angle_error * T_SAMPLE;
    
    // Implement PID control logic here (simplified version)
    float P_term = Kp_alpha * angle_error;
    float I_term = Ki_alpha * integral_of_angle_error;
    float D_term = -Kd_alpha * alpha_dot;
    
    // Calculate control output, which is the desired wheel velocity
    float vRad_soll = P_term + I_term + D_term;
	float vRad_R = vRad_soll+left;
	float vRad_L = vRad_soll-left;
	motor_setVel(vRad_L, vRad_R);

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


			// PID controller constants from script
			//float Kp_v = 0.3531f;
			//float Ki_v = 0.6757f;
			//float Kd_v = 0.0006f;

			// PID controller tuned
			float Kp_v = 0.5991f;
			float Ki_v = 0.7757f;
			float Kd_v = 0.0156f;

			float l_v = 5.0f; // distance between axis and point to control around

			//werte für die tasten sind im header definiert

			// werte über UART empfangen und in variable speichern
			uint8_t data = uart_getch();

			switch (data)
			{
			case 'w':
				forward += 1.0;
				break;
			case 's':
				forward -= 1.0;
				break;
			case 'a':
				left += 1.0;
				break;
			case 'd':
				left -= 1.0;
				break;
			case ' ':
				forward = 0;
				left = 0;
				right = 0;
				break;

			default:
				break;
			}


			float vSoll = forward; // Desired velocity
			
			float drad = alpha_dot * (M_PI / 180.0); // Convert angle to radians 

			float velocity_error =  vSoll -  (vRad + (l_v + (D_RAD_L/2.0)) * drad); // Calculate velocity error

			// without integral windup
			integral_of_velocity_error += velocity_error * T_SAMPLE;

        	float derivative_of_velocity_error = (velocity_error - previous_velocity_error) / T_SAMPLE;
        	previous_velocity_error = velocity_error; // Update previous error for next iteration

			float P_term_velocity = Kp_v * velocity_error;
			float I_term_velocity = Ki_v * integral_of_velocity_error; 
			float D_term_velocity = Kd_v * derivative_of_velocity_error; 

			float alpha_soll = P_term_velocity + I_term_velocity + D_term_velocity;

			reglerBalancieren_winkelRegler(alpha, alpha_dot, alpha_soll); // Inner loop

			//velocity_error_prev = velocity_error;

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

	// set changing values to zero

	integral_of_angle_error = 0; // Integral of angle error
	integral_of_velocity_error = 0; // Initialize the static variable
	previous_velocity_error = 0; // To store previous error for derivative calculation

	z1 = 0; // PT2-Zustand 1
	z2 = 0; // PT2-Zustand 2



	// Read accelerometer and gyroscope data
    int16_t accData[3]; // Array to hold accelerometer data
    acc_getData(accData); // Fetch the accelerometer data


	float alpha_acc = atan2f(accData[2], accData[0]);
    alpha_acc *= (180.0f / M_PI); // Convert to degrees

	cumulativeAngleGyro = alpha_acc; // Set the initial angle to the accelerometer angle

	/* CODE END */
}

/* Ausf�hrung des Reglers zum Balancieren 
	- Sensordaten lesen 
	- Komplement�rfilter ausf�hren
	- Regler ausf�hren */
void reglerBalancieren_regelung(void) 
{
	/* CODE START */
	float alphaCompFilter; // Complementary filter angle
	float alpha_dot; // Angular velocity
	reglerBalancieren_komplementaerFilter(&alphaCompFilter, &alpha_dot); // Execute the complementary filter

	//reglerBalancieren_winkelRegler(alphaCompFilter, alpha_dot, 0); // Execute the angle controller
	float vL, vR;
	motor_getVel(&vL, &vR); // Get the velocity velo

	float v_rad = (vL + vR) / 2.0f; // Calculate the average velocity

	reglerBalancieren_geschwindigkeitsRegler(v_rad, alphaCompFilter, alpha_dot); // Execute the angle controller

	//char buffer[128];
    //snprintf(buffer, sizeof(buffer), "vRad = %3.5f, Alpha: %3.5f\n\r", v_rad, alphaCompFilter);
    //uart_puts((uint8_t*)buffer);
	
	/* CODE END */
}

/* CODE START */
/* CODE END */
