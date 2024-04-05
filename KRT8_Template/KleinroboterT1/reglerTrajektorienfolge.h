#ifndef REGLERTRAJEKTORIENFOLGE_H_
#define REGLERTRAJEKTORIENFOLGE_H_

#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <util/twi.h>

#include "routines/trajektorieVorgabe.h"
#include "routines/uart.h"
#include "motor.h"
#include "main.h"

/* Funktionen */
void reglerTrajektorienfolge_init(void);
void reglerTrajektorienfolge_beobachter(float* u1_B, float* u2_B, float* x1_B, float* x2_B, float* x3_B);
void reglerTrajektorienfolge_steuerung(void);
void reglerTrajektorienfolge_regelung(float u1_B, float u2_B, float x1_B, float x2_B, float x3_B);

#endif /* REGLERTRAJEKTORIENFOLGE_H_ */