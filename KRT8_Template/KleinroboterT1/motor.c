#include "motor.h"

/* CODE START */
/* CODE END */

/* Initialisierungsroutine für die Motoransteuerung
	Hier soll z.B. der Timer für die Motoren konfiguriert
	und die H-Brücke initialisiert werden */
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
			pwm_duty -	Vorzeichenbehafteter Dutycycle (-1 voll zurück, +1 voll vorwärts) */
void motor_pwm(uint8_t motorId, float pwm_duty)
{
	/* CODE START */
	/* CODE END */
}

/* Routine zum Auslesen der Radgeschwindigkeiten in cm/s über die Decoder
	OUTPUT: vMessLinks - Geschwindigkeit des linken Rades (cm/s)
			vMessRechts - Geschwindigkeit des rechten Rades (cm/s) */
void motor_getVel(float* vMessLinks, float* vMessRechts)
{
	/* CODE START */
	/* CODE END */
}

/* CODE START */
/* CODE END */