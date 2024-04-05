#include "motor.h"

/* CODE START */
/* CODE END */

/* Initialisierungsroutine f�r die Motoransteuerung
	Hier soll z.B. der Timer f�r die Motoren konfiguriert
	und die H-Br�cke initialisiert werden */
void motor_init(void) 
{
	/* CODE START */
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
	/* CODE END */
}

/* Routine zur Ansteuerung der Motoren per %DutyCycle 
	INPUT:  motorId -	Flag, welche das anzusteuerende Rad angibt
			pwm_duty -	Vorzeichenbehafteter Dutycycle (-1 voll zur�ck, +1 voll vorw�rts) */
void motor_pwm(uint8_t motorId, float pwm_duty)
{
	/* CODE START */
	/* CODE END */
}

/* Routine zum Auslesen der Radgeschwindigkeiten in cm/s �ber die Decoder
	OUTPUT: vMessLinks - Geschwindigkeit des linken Rades (cm/s)
			vMessRechts - Geschwindigkeit des rechten Rades (cm/s) */
void motor_getVel(float* vMessLinks, float* vMessRechts)
{
	/* CODE START */
	/* CODE END */
}

/* CODE START */
/* CODE END */