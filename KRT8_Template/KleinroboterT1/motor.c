#include "motor.h"

/* CODE START */




const float U_nom = 8.2f;
/* CODE END */

/* Initialisierungsroutine f�r die Motoransteuerung
	Hier soll z.B. der Timer f�r die Motoren konfiguriert
	und die H-Br�cke initialisiert werden */
void motor_init(void) 
{
	/* CODE START */
	
    /* Motoransteuerung */

    DDRC |= (1<<PC7);	// Motor Links L_phase
    DDRC |= (1<<PC6);	// Motor rechts R_phase
    DDRD |= (1<<PD5);	// Motor links L_enbl
    DDRD |= (1<<PD4);	// Motor rechts R_enbl

    //Set Timer1 for Fast PWM mode with ICR1 as TOP
    TCCR1A = (1 << WGM11) | (1 << COM1A1) | (1<<COM1B1); // WGM11:0 = 10 for Fast PWM, COM1A1:0 = 10 for non-inverting mode
    TCCR1B = (1 << WGM12) | (1 << WGM13); // WGM13:2 = 111 for Fast PWM with ICR1 as TOP
    
    //Set the prescaler and start the timer

    TCCR1B |= (1 << CS10); // CS12:0 = 010 for prescaler 8

    //Set ICR1 to define the PWM period to achieve a ~10 kHz frequency
    ICR1 = MAX_PWM;

    //start the motor for testing
    //OCR1A = 50; // Set the duty cycle to 50
	//OCR1B = 50;


	/* CODE END */
}

/* Routine zur Ansteuerung der Motoren mit Kennlinienkorrektur */
void motor_setVel(float vSollLinks, float vSollRechts)		// vSollLinks und vSollRechts in cm/s
{
	/* CODE START */

	float U_nom = 8.3; // [V] Nominal voltage
	const float mv_L = 8.097e-3;
	const float mv_R = 8.097e-3;
	const float cv_L = 1.214e-2;
	const float cv_R = 1.214e-2;

	/*
	const float mv_L = 0.0067;
	const float cv_L = 0.0556;
	const float mv_R = 0.0064;
	const float cv_R = 0.0747;
	*/

	float U_aktuell;
	U_aktuell = akku_voltage(); //

	float sgn_L = 0;
	float sgn_R = 0;

	if(vSollLinks > 0) sgn_L = -1;
	else if(vSollLinks < 0) sgn_L = 1;

	if(vSollRechts > 0) sgn_R = 1;
	else if(vSollRechts < 0) sgn_R = -1;

	// TODO: add pwm = 0 for small velocities

    // Convert the target speeds to PWM duty cycle using the motor characteristics.
    float pwmDutyL = (U_nom/U_aktuell) * (mv_L * vSollLinks + cv_L * sgn_L);
    float pwmDutyR = (U_nom/U_aktuell) * (mv_R * vSollRechts + cv_R * sgn_R);


    // The direction is determined by the sign of vSollLinks and vSollRechts
    // If negative, we need to drive the motor in reverse.

    // Apply the PWM duty cycle to the motors, ensuring direction is handled.
    motor_pwm(FLAG_L, pwmDutyL);
    motor_pwm(FLAG_R, pwmDutyR);

	/* CODE END */		
}

/* Routine zur manuellen Steuerung per UART */
void motor_manualCtrl(void) 
{
	/* CODE START */
	// Hier soll die manuelle Steuerung der Motoren �ber UART implementiert werden
	// WASD soll zur steuerung verwendet werden, wobei ein gedrückt halten der Taste incrementell einen Wert erhöht
	// das drücken der Leertaste soll die Motoren stoppen und alle werte auf null zurück setzten
	// die Werte sollen dann in motor_pwm übergeben werden
	// jeder wert hat einen Bereich von -1 bis 1, und 0 lässt die motoren stehen

	// Werte für die Motoren
	//pwm_duty_L = 0;
	//pwm_duty_R = 0;

	float vel_L = 0;
	float vel_R = 0;

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

	// Berechnung der PWM Werte
	//pwm_duty_R = forward - backward + left - right;
	//pwm_duty_L = forward - backward - left + right;

	//vel_R = forward - backward + left - right;
	//vel_L = forward - backward - left + right;

	vel_R = forward -left;
	vel_L = forward +left;

	/*
	char buffer[150];

	float v_ist_L, v_ist_R;
	motor_getVel(&v_ist_L, &v_ist_R);

	snprintf(buffer, sizeof(buffer), "L_ist: %.2f, L_soll: %.2f , R_ist: %.2f, R_soll: %.2f \r\n", v_ist_L, vel_L, v_ist_R, vel_R);
	uart_puts((uint8_t*)buffer);
	*/

	// Werte an die Motoren übergeben
	//motor_pwm(FLAG_L, vel_L);
	//motor_pwm(FLAG_R, vel_R);

	//motor_pwm(FLAG_L, 50);
	//motor_pwm(FLAG_R, 50);

	//velocity control

	motor_setVel(vel_L, vel_R);

	/* CODE END */
}

/* Routine zur Ansteuerung der Motoren per %DutyCycle 
	INPUT:  motorId -	Flag, welche das anzusteuerende Rad angibt
			pwm_duty -	Vorzeichenbehafteter Dutycycle (-1 voll zur�ck, +1 voll vorw�rts) */
void motor_pwm(uint8_t motorId, float pwm_duty)
{
	/* CODE START */

	if(pwm_duty > 1) pwm_duty = 1;
	if(pwm_duty < -1) pwm_duty = -1;

	if(motorId == FLAG_L)
	{
		if(pwm_duty > 0)
		{
			PORTC |= (1<<PC7); // Set the direction to backward
		}
		else
		{
			PORTC &= ~(1<<PC7); // Set the direction to forward
		}
		OCR1A = fabs(pwm_duty)*MAX_PWM; // Set the duty cycle
	}
	else if(motorId == FLAG_R)
	{
		if(pwm_duty < 0)
		{
			PORTC |= (1<<PC6); // Set the direction to backward
		} 
		else
		{
			PORTC &= ~(1<<PC6); // Set the direction to forward
		}
		OCR1B = fabs(pwm_duty)*MAX_PWM; // Set the duty cycle
	}
	/* CODE END */
}

/* Routine zum Auslesen der Radgeschwindigkeiten in cm/s �ber die Decoder
	OUTPUT: vMessLinks - Geschwindigkeit des linken Rades (cm/s)
			vMessRechts - Geschwindigkeit des rechten Rades (cm/s) */
void motor_getVel(float* vMessLinks, float* vMessRechts)
{
	/* CODE START */

	// Geschwindigkeit des linken und rechten Rades in cm/s

    static int32_t lastCountLeft = 0;
    static int32_t lastCountRight = 0;

    int32_t currentCountLeft = qdec_getCounts(FLAG_SPI_QDEC_L);
    int32_t currentCountRight = qdec_getCounts(FLAG_SPI_QDEC_R);

    // Berechnen Sie die zurückgelegte Strecke für jedes Rad
    float distancePerCountLeft = (PI * D_RAD_L) / COUNTS_PER_REV;
    float distancePerCountRight = (PI * D_RAD_R) / COUNTS_PER_REV; 

    int32_t deltaCountLeft = currentCountLeft - lastCountLeft;
    int32_t deltaCountRight = currentCountRight - lastCountRight;

    // Geschwindigkeit in cm/s berechnen
    *vMessLinks = (distancePerCountLeft * deltaCountLeft) / T_SAMPLE ;
    *vMessRechts = (distancePerCountRight * deltaCountRight) / T_SAMPLE;

    // Aktualisieren der letzten Count-Werte für die nächste Messung
    lastCountLeft = currentCountLeft;
    lastCountRight = currentCountRight;
}

	/* CODE END */


/* CODE START */
/* CODE END */