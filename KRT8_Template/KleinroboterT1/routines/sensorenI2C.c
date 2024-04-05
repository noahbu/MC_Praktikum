#include "sensorenI2C.h"

/* Deklariere und Initialisiere den Gyro-Offset zu null */
int16_t gyroOffset[3] = {0,0,0};

/* Routine zum Initialisieren des Gyros */
void gyro_init(void)
{
	/* Konfiguriere den Gyro */
	TWBR = ((F_CPU/I2C_SPEED) - 16)/2;	// I2C-Register konfigureren
	
	gyro_write(21, 0);				// 1kHz interne Abtastung des Analoggyros
	gyro_write(22, 0b00010011);		// no sync, 1000 deg/s, 42 Hz Lowpass
	
	/* Kalibriere den Gyro (setzt den Gyro-Offset) */
	gyro_calibrate();
}

/* Routine um ein Datenbyte über I2C in ein Sensorregister zu schreiben */
void gyro_write(uint8_t Register, uint8_t data)
{
	/* Sende Start-Kondition (Datenrichtung Master->Slave) */
	gyro_sendStart(0);
	
	/* Sende Addresse des Registers */
	TWDR = Register;
	TWCR = (1<<TWINT) | (1<<TWEN);	
	/* Warte auf Acknowlegde des Gyros */
	while (!(TWCR & (1<<TWINT)));
		
	/* Sende die Daten */
	TWDR = data;
	TWCR = (1<<TWINT) | (1<<TWEN);	
	/* Warte auf Acknowlegde des Gyros */
	while (!(TWCR & (1<<TWINT)));
	
	/* Sende Stop-Kondition */
	gyro_sendStop();
	
}

/* Routine zum Auslesen von n-Datenbytes aus einem Sensorregister */
void gyro_read(uint8_t Register, uint8_t *data, uint8_t n_bytes)
{	
	/* Sende Start-Kondition (Datenrichtung Master->Slave) */
	gyro_sendStart(0);
	
	/* Sende Addresse des Registers */
	TWDR = Register;
	TWCR = (1<<TWINT) | (1<<TWEN);
	/* Warte auf Acknowlegde des Gyros */
	while (!(TWCR & (1<<TWINT)));
	
	/* Sende Start-Kondition (Datenrichtung Slave->Master) */
	gyro_sendStart(1);
	
	/* Lese die Daten */
	for(int16_t i=0;i<n_bytes;i++)
	{
		/* Sende Acknowledge an den Gyro */
		TWCR = (1<<TWINT)|(1<<TWEN)|((i==n_bytes-1) ? 0 : (1<<TWEA));
		/* Warte bis die Übertragung komplett ist */
		while (!(TWCR & (1<<TWINT)));
		data[i] = TWDR;
	}
	
	/* Sende Stop-Kondition */
	gyro_sendStop();
}

/* Stelle die Stopkondition her */
void gyro_sendStop(void)
{
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);
	/* Warten */
	while(TWCR & (1<<TWSTO));
}

/* Routine zum Verbindungsaufbau und Festlegen der Datenrichtung */
void gyro_sendStart(uint8_t RW_bit)
{
	/* RW_bit = 0: Write Master -> Slave
	* RW_bit = 1: Read  Master <- Slave
	*/
		/* Sende Start-Kondition */
		TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
		
		/* Warten bis das TWINT Flag gesetzt wurde, dies zeigt, dass der "START" verschickt wurde. */
		while (!(TWCR & (1<<TWINT)));
		
		/* Sende Adresse + WRITE bit = 0 */
		TWDR = ITG_I2C_ADR<<1|RW_bit;
		TWCR = (1<<TWINT)|(1<<TWEN);
		
		/* Warten auf Acknowledge */
		while (!(TWCR & (1<<TWINT)));
}

/* Routine zum Auslesen der Drehratendaten aus dem Gyro */
void gyro_getData(int16_t *pGyroData)
{
	uint8_t Buffer[6];	// Buffer zum Einlesen der Daten
	gyro_read(29, Buffer, 6);	// Daten über I2C einlesen

	pGyroData[0] = ((int)((Buffer[0]<<8) | Buffer[1]) - gyroOffset[0]); // Datensatz für für die x-Achse
	pGyroData[1] = ((int)((Buffer[2]<<8) | Buffer[3]) - gyroOffset[1]); // Datensatz für für die y-Achse
	pGyroData[2] = ((int)((Buffer[4]<<8) | Buffer[5]) - gyroOffset[2]); // Datensatz für für die z-Achse
}

/* Routine zum Kalibrieren des Gyro (Bestimmung der Drehratenoffsets der Achsen) */
void gyro_calibrate(void)
{	
	int16_t i = 0;	// Schleifenzähler
	int16_t gyroData[3];	// Buffervariablen für die Drehraten
	int32_t x=0,y=0,z=0;	// Hilfsvariablen zur Offsetberechnung

	/* Auslesen von 64 Datensätzen aus dem Gyro */
	for (i = 0; i < 64; i++)
	{
		gyro_getData(gyroData);
		x += gyroData[0];
		y += gyroData[1];
		z += gyroData[2];
		_delay_ms(10);
	}
	
	/* Berechnung des Offsets (Summe / 64) */
	gyroOffset[0] = x >> 6;
	gyroOffset[1] = y >> 6;
	gyroOffset[2] = z >> 6;
}