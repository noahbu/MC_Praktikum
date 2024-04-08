#include "motor.h"

/* CODE START */
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
	float pwm_duty_L = 0;
	float pwm_duty_R = 0;

	//werte für die tasten sind im header definiert

	// werte über UART empfangen und in variable speichern
	uint8_t data = uart_getch();

	switch (data)
	{
	case 'd':
		forward += 0.01;
		break;
	case 'a':
		backward += 0.01;
		break;
	case 'w':
		left += 0.01;
		break;
	case 's':
		right += 0.01;
		break;
	case ' ':
		forward = 0;
		backward = 0;
		left = 0;
		right = 0;
		break;

	default:
		break;
	}

	// Berechnung der PWM Werte
	pwm_duty_R = forward - backward + left - right;
	pwm_duty_L = forward - backward - left + right;



	//char buffer[64]; // Buffer to hold the formatted string

    // Format and send pwm_duty_L
    //sprintf(buffer, "%f", pwm_duty_L);
    //uart_puts((uint8_t *)buffer);
    //uart_puts((uint8_t *)"\n\r"); // New line and carriage return for clarity

    // Format and send pwm_duty_R
    //sprintf(buffer, "%f", pwm_duty_R);
    //uart_puts((uint8_t *)buffer);
    //uart_puts((uint8_t *)"\n\r"); // New line and carriage return

	// Werte an die Motoren übergeben
	motor_pwm(FLAG_L, pwm_duty_L);
	motor_pwm(FLAG_R, pwm_duty_R);

	/* CODE END */
}

/* Routine zur Ansteuerung der Motoren per %DutyCycle 
	INPUT:  motorId -	Flag, welche das anzusteuerende Rad angibt
			pwm_duty -	Vorzeichenbehafteter Dutycycle (-1 voll zur�ck, +1 voll vorw�rts) */
void motor_pwm(uint8_t motorId, float pwm_duty)
{
	/* CODE START */
/*
angesprochen werden können. Das Argument sideId ist hierbei ein unsigned integer, der
die anzusprechende Seite identifiziert, z. B. 0 für links und 1 für rechts. Das Argument
pwm_duty gibt den kommandierten PWM-duty-cycle an und hat einen Wertebreich von
-1 (volle Rückwärtsfahrt) bis +1 (volle Vorwärtsfahrt).*/

	if(motorId == FLAG_L)
	{
		if(pwm_duty < 0)
		{
			PORTC |= (1<<PC7); // Set the direction to backward
			OCR1A = -pwm_duty*MAX_PWM; // Set the duty cycle
		}
		else
		{
			PORTC &= ~(1<<PC7); // Set the direction to forward
			OCR1A = pwm_duty*MAX_PWM; // Set the duty cycle
		}
	}
	else if(motorId == FLAG_R)
	{
		if(pwm_duty < 0)
		{
			PORTC |= (1<<PC6); // Set the direction to backward
			OCR1B = -pwm_duty*MAX_PWM; // Set the duty cycle
		}
		else
		{
			PORTC &= ~(1<<PC6); // Set the direction to forward
			OCR1B = pwm_duty*MAX_PWM; // Set the duty cycle
		}
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
	
	/* CODE END */
}

/* CODE START */
/* CODE END */