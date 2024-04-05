#ifndef MAIN_H_
#define MAIN_H_

/* Ausgabe LEDs benennen */
#define LED_GRUEN PA2
#define LED_GELB PA1
#define LED_ROT PA0
#define LED_GRUENB PB1

/* Samplezeit setzen */
#define T_SAMPLE (0.01f)

/* Makros für die Regleraktivierung */
#define MANUELLE_STEUERUNG 0
#define BALANCIERREGLER 1
#define TRAJEKTORIENREGLER 2
#define TESTING 3

/* Konstanten */
#define PI (3.14159265f)  // Kreiskonstante

/* Winkel Umrechnungsfaktoren */
#define RAD_TO_DEG (180.0f/PI)
#define DEG_TO_RAD (PI/180.0f)

#endif /* MAIN_H_ */