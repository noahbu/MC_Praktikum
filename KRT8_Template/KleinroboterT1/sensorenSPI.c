#include "sensorenSPI.h"

// #define _OLD_SPI_

/* Initialisierung der SPI Schnittstelle zu den Decodern und dem Accelerometer */
void spi_init(void)
{
	/* CODE START */
	/* CODE END */
}

/* Quadratur-Decoder initialisieren */
void qdec_init(void)
{	
	/* links */
	qdec_writeRegister(FLAG_SPI_QDEC_L, WRITE_MDR1, DIS_CNTR);			// Zählen ausschalten
	qdec_writeRegister(FLAG_SPI_QDEC_L, WRITE_MDR0, QUADRX4|FREE_RUN|DISABLE_INDX|FILTER_1); // Konfigurieren
	qdec_writeCommand(FLAG_SPI_QDEC_L, CLR_CNTR);							// Zähler auf null setzen
	qdec_writeRegister(FLAG_SPI_QDEC_L, WRITE_MDR1, BYTE_4|EN_CNTR);		// 4 Bytemodus einschalten und Zähler einschalten
	/* rechts */
	qdec_writeRegister(FLAG_SPI_QDEC_R, WRITE_MDR1, DIS_CNTR);			// Zählen ausschalten
	qdec_writeRegister(FLAG_SPI_QDEC_R, WRITE_MDR0, QUADRX4|FREE_RUN|DISABLE_INDX|FILTER_1); // Konfigurieren
	qdec_writeCommand(FLAG_SPI_QDEC_R, CLR_CNTR);							// Zähler auf null setzen
	qdec_writeRegister(FLAG_SPI_QDEC_R, WRITE_MDR1, BYTE_4|EN_CNTR);		// 4 Bytemodus einschalten und Zähler einschalten
}

/* Schreiben eines Registers auf den Quadratur-Decodern 
	INPUT:  - spiSensorId:	Auswahl des Decoders welcher angesprochen wird (z.B. FLAG_SPI_QDEC_L)
			- registerByte: Das zeschreibenden Registers (z.B. WRITE_MDR0)
			- data:			Daten die in das Register geschrieben werden sollen */ 
void qdec_writeRegister(uint8_t spiSensorId, uint8_t registerByte, uint8_t data)
{
	/* CODE START */
	/* CODE END */
}

/* Senden eines Kommandos an die Quadratur-Decoder 
	INPUT:  - spiSensorId:	Auswahl des Decoders welcher angesprochen wird (z.B. FLAG_SPI_QDEC_L)
			- comand:		Das zu sendende Kommando (z.B. CLR_CNTR) */
void qdec_writeCommand(uint8_t spiSensorId, uint8_t command)
{
	/* CODE START */
	/* CODE END */
}


/* Gebe die gezählten Ticks zurück
	INPUT:	- spiSensorId:  Auswahl des Decoders welcher angesprochen wird (z.B. FLAG_SPI_QDEC_L) 
	OUTPUT: - counts:		Anzahl der gezählten Ticks */
int32_t qdec_getCounts(uint8_t spiSensorId)
{
	/* CODE START */
	/* CODE END */
}

/* Auswahl eines Slaves mit dem kommuniziert werden soll
	INPUT:	- spiSensorId:  Auswahl des Decoders welcher ausgewählt werden soll (z.B. FLAG_SPI_ACC) */   
void spi_select(uint8_t spiSensorId)
{
	/* CODE START */
	/* CODE END */
}

/* Sende Daten an den aktuell aktiven Slave und lese mepfangene Daten sofort aus */
uint8_t spi_sendAndRead(uint8_t data) {
	/* CODE START */
	/* CODE END */
}

/* Initialisierung des Beschleunigungssensors */
void acc_init(void)
{
	// Konfiguriere den Beschleunigungssensor
	acc_writeRegister(ADXL_BW_RATE,		0b00001010 ); // Output rate 100 Hz, Bandwidth 50 Hz
	acc_writeRegister(ADXL_DATA_FORMAT, 0b00001011 ); // Volle Auflösung, Range +-16g !!
		
	// Beschleunigungssensor aus- und wieder einschalten
	acc_writeRegister(ADXL_POWER_CTL,	0b00000000 ); // Alles aus
	acc_writeRegister(ADXL_POWER_CTL,	0b00010000 ); // AUTO_SLEEP modus aktivieren
	acc_writeRegister(ADXL_POWER_CTL,	0b00001000 ); // MEASURE modus aktivieren
	
	// Kurz warten
	_delay_ms(100);
}

/* Schreiben eines Registers auf den Beschleunigungssensoren
	INPUT:  - registerByte: Das zeschreibenden Registers (z.B. ADXL_BW_RATE)
			- data:			Daten die in das Register geschrieben werden sollen */
void acc_writeRegister(uint8_t registerByte, uint8_t data)
{
	/* CODE START */
	/* CODE END */
}

/* Auslesen des Beschleunigungssensors
	OUTPUT: - int accData[3]: Messwerte des Sensors in x (accData[0]), y (accData[1]), z (accData[2]) 
	TIPP zum Auslesen: vgl. 'ADXL345 Quick start guide.pdf', Seite 4, Fig 7 */
void acc_getData(int16_t *accData)
{
	/* CODE START */
	/* CODE END */
}

/* CODE START */
/* CODE END */