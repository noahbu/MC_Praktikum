#ifndef REGLERBALANCIEREN_H_
#define REGLERBALANCIEREN_H_

#include <math.h>

#include "routines/sensorenI2C.h"
#include "routines/uart.h"
#include "routines/akku.h"
#include "motor.h"
#include "sensorenSPI.h"
#include "main.h"

/* Konfiguration des Abschaltmechanismus */
#define ALPHA_ABS_DEACTIVATE 45.0f	// Abschalten des Reglers bei Überschreiten von ALPHA_MAX
#define ALPHA_ABS_ACTIVATE 20.0f	// (Wieder-)Einschalten des Reglers bei Unterschreiten von ALPHA_ABS_ACTIVATE
/* CODE START */
/* CODE END */

/* Funktionen um den Regler zu initialisieren und auszuführen */
void reglerBalancieren_init(void);
void reglerBalancieren_regelung(void);

/* Hilfsroutinen für die Ausführung des Reglers */
float reglerBalancieren_PT2(float x);
void reglerBalancieren_komplementaerFilter(float* alphaCompFilter, float* alpha_dot);
void reglerBalancieren_winkelRegler(float alpha, float alpha_dot, float alpha_soll);
void reglerBalancieren_geschwindigkeitsRegler(float vRad, float alpha, float alpha_dot);
/* CODE START */
/* CODE END */

#endif /* REGLERBALANCIEREN_H_ */