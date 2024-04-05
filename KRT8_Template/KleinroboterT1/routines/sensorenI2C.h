#ifndef SENSORENI2C_H_
#define SENSORENI2C_H_

#include <avr/io.h>
#include <util/twi.h>
#include <util/delay.h>

#define I2C_SPEED 400000	// Fast-Modus, 400kHz Clockspeed
#define ITG_I2C_ADR 0b01101000	// I2C-Adresse des Gyros laut Gyrodatenblatt
#define GYRO_LSB_TO_DEG (1.0/32.8)  // Umrechnung von LSB in Grad


/* Routine zum Initialisieren des Gyros */
void gyro_init(void);

/* Routine um ein Datenbyte über I2C in ein Sensorregister zu schreiben */
void gyro_read(uint8_t Register, uint8_t *data, uint8_t n_bytes);
/* Routine zum Auslesen von n-Datenbytes aus einem Sensorregister */
void gyro_write(uint8_t Register, uint8_t data);

/* Stelle die Stopkondition her */
void gyro_sendStop(void);
/* Routine zum Verbindungsaufbau und Festlegen der Datenrichtung */
void gyro_sendStart(uint8_t RW_bit);

/* Routine zum Auslesen der Drehratendaten aus dem Gyro */
void gyro_getData(int *pGyroData)
/* Routine zum Kalibrieren des Gyro (Bestimmung der Drehratenoffsets der Achsen) */;
void gyro_calibrate(void);

#endif /* SENSORENI2C_H_ */