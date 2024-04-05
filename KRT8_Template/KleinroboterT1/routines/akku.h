#ifndef AKKU_H_
#define AKKU_H_

#define VREF 3.25	// Referenzspannung f�r den Spannungsteiler des ADC3	
#define SPANNUNGSTEILER 9.18	// Spannungsteilerverh�ltnis laut Schaltplan (18k+2.2k)/(2.2k)
#define V_LOW 7.2	// Spannung, ab der die Akku�berpr�fung warnen soll

#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "uart.h"

/* Initialisierungsroutine f�r das Auslesen der Akkuspannung */
void akku_init();
/* Routine zum  Abfragen der aktuellen Akkuspannung */
float akku_voltage();
/* Routine zur �berp�fung, ob die Akkuspannung nicht unterhalb des kritischen Wertes liegt */
void akku_check();

#endif /* AKKU_H_ */