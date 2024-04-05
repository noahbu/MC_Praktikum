#ifndef MOTOR_H_
#define MOTOR_H_

#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <util/twi.h>

#include "routines/akku.h"
#include "main.h"
#include "sensorenSPI.h"

/* Modellkonstanten */
#define D 9.9f			// [cm] Radabstand
#define D_RAD_L 6.6f	// [cm] Durchmesser linkes Rad
#define D_RAD_R 6.6f	// [cm] Durchmesser rechtes Rad

/* links-rechts Flags zur Motoransteuerung (motorId) */
#define FLAG_L 0
#define FLAG_R 1

/* CODE START */
#define MAX_PWM 99;


float forward ;
float backward;
float left;
float right;

/* CODE END */

/* Initialisierungsroutine f�r die Motoransteuerung	*/
void motor_init(void);
/* Routine zur Ansteuerung der Motoren mit Kennlinienkorrektur */
void motor_setVel(float vSollLinks, float vSollRechts);
/* Routine zur manuellen Steuerung per UART */
void motor_manualCtrl(void);
/* Routine zum Auslesen der Radgeschwindigkeiten in cm/s �ber die Decoder */
void motor_getVel(float* vMessLinks, float* vMessRechts);

/* Routine zur Ansteuerung der Motoren per %DutyCycle */
void motor_pwm(uint8_t motorId, float pwm_duty);

/* CODE START */

/* CODE END */

#endif /* MOTOR_H_ */