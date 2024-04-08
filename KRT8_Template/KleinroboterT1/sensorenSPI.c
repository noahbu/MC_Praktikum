#include "sensorenSPI.h"

// #define _OLD_SPI_

/* Initialisierung der SPI Schnittstelle zu den Decodern und dem Accelerometer */
void spi_init(void)
{
	/* CODE START */
	
	//Aufgabenbeschreibung
	// ausgangspins setzten: SCK MOSI, Select leitungen (quadratur decoder und beschleunigungssensor)
	// SPI anschalten
	// MC als als Master konfigurieren
	// zu beginn nmodus 0 setzen (CPOL = 0, CPHA = 0)
	// aktiv keinen slave auswählen (alles SS leitungen auf 1)

	// Ausgangspins setzen
	DDRB |= (1<<PB7);	// SCK
	DDRB |= (1<<PB5);	// MOSI
	DDRB |= (1<<PB3);	// SS_accelerator
	DDRB |= (1<<PB0);	// SS_Qdec_R
	DDRB |= (1<<PB4);	// SS_Qdec_L

	// als master konfigurieren
	SPCR |= (1<<MSTR);

	// SPI anschalten
	SPCR |= (1<<SPE);

	// nmodus 0 setzen, CPOL = 0, CPHA = 0. Ist initial auch auf null, aber für bessere versätndlichkeit noch auf null setzen
	SPCR &= ~(1<<CPOL);
	SPCR &= ~(1<<CPHA);

	// SS leitungen auf 1 setzen
	PORTB |= (1<<PB3) | (1<<PB4) | (1<<PB0);
	 
	

	/* CODE END */
}

/* Quadratur-Decoder initialisieren */
void qdec_init(void)
{	
	
	/* links */
	
	qdec_writeRegister(FLAG_SPI_QDEC_L, WRITE_MDR1, DIS_CNTR);			// Z�hlen ausschalten
	qdec_writeRegister(FLAG_SPI_QDEC_L, WRITE_MDR0, QUADRX4|FREE_RUN|DISABLE_INDX|FILTER_1); // Konfigurieren
	qdec_writeCommand(FLAG_SPI_QDEC_L, CLR_CNTR);							// Z�hler auf null setzen
	qdec_writeRegister(FLAG_SPI_QDEC_L, WRITE_MDR1, BYTE_4|EN_CNTR);		// 4 Bytemodus einschalten und Z�hler einschalten
	
	/* rechts */
	
	qdec_writeRegister(FLAG_SPI_QDEC_R, WRITE_MDR1, DIS_CNTR);			// Z�hlen ausschalten
	qdec_writeRegister(FLAG_SPI_QDEC_R, WRITE_MDR0, QUADRX4|FREE_RUN|DISABLE_INDX|FILTER_1); // Konfigurieren
	qdec_writeCommand(FLAG_SPI_QDEC_R, CLR_CNTR);							// Z�hler auf null setzen
	qdec_writeRegister(FLAG_SPI_QDEC_R, WRITE_MDR1, BYTE_4|EN_CNTR);		// 4 Bytemodus einschalten und Z�hler einschalten
	
}

/* Schreiben eines Registers auf den Quadratur-Decodern 
	INPUT:  - spiSensorId:	Auswahl des Decoders welcher angesprochen wird (z.B. FLAG_SPI_QDEC_L)
			- registerByte: Das zeschreibenden Registers (z.B. WRITE_MDR0)
			- data:			Daten die in das Register geschrieben werden sollen */ 
void qdec_writeRegister(uint8_t spiSensorId, uint8_t registerByte, uint8_t data)
{
	/* CODE START */

	// wird in der init Funktion aufgerufen um die Decoder zu konfigurieren
	// SS leitung des gewünschten Decoders aktivieren
	// Daten senden
	// SS leitung des gewünschten Decoders deaktivieren

	spi_select(spiSensorId);
	spi_sendAndRead(registerByte);
	spi_sendAndRead(data);
	spi_select(FLAG_SPI_NONE);
	
	/* CODE END */
}

/* Senden eines Kommandos an die Quadratur-Decoder 
	INPUT:  - spiSensorId:	Auswahl des Decoders welcher angesprochen wird (z.B. FLAG_SPI_QDEC_L)
			- comand:		Das zu sendende Kommando (z.B. CLR_CNTR) */
void qdec_writeCommand(uint8_t spiSensorId, uint8_t command)
{
	/* CODE START */
	// wird in der init Funktion aufgerufen um die Decoder zu konfigurieren

	spi_select(spiSensorId);
	spi_sendAndRead(command);
	spi_select(FLAG_SPI_NONE);

	/* CODE END */
}


/* Gebe die gez�hlten Ticks zur�ck
	INPUT:	- spiSensorId:  Auswahl des Decoders welcher angesprochen wird (z.B. FLAG_SPI_QDEC_L) 
	OUTPUT: - counts:		Anzahl der gez�hlten Ticks */
int32_t qdec_getCounts(uint8_t spiSensorId)
{
	/* CODE START */

	// SS leitung des gewünschten Decoders aktivieren
	// Daten senden
	// Daten empfangen

	spi_select(spiSensorId);
	//spi_sendAndRead(LOAD_OTR); // If required, depends on your setup and need
	spi_sendAndRead(READ_CNTR); // Assuming this is the command to start reading, replace with actual command if different

	// Cast to uint32_t before shifting to avoid warning
	int32_t counts = (uint32_t)spi_sendAndRead(0) << 24; // MSB
	counts |= (uint32_t)spi_sendAndRead(0) << 16;
	counts |= (uint32_t)spi_sendAndRead(0) << 8;
	counts |= (uint32_t)spi_sendAndRead(0); // LSB
	spi_select(FLAG_SPI_NONE);

	if (spiSensorId == FLAG_SPI_QDEC_R) {
        counts = -counts;
    }

	return counts;

	/* CODE END */
}

/* Auswahl eines Slaves mit dem kommuniziert werden soll
	INPUT:	- spiSensorId:  Auswahl des Decoders welcher ausgew�hlt werden soll (z.B. FLAG_SPI_ACC) */   
void spi_select(uint8_t spiSensorId)
{
	/* CODE START */

	// SS leitung des gewünschten Decoders aktivieren
	// Data order: MSB / LSB first
	// Mode 0: CPOL = 0, CPHA = 0
	// SS leitung des gewünschten Decoders deaktivieren
	

	if(spiSensorId == FLAG_SPI_QDEC_L)
	{
		PORTB &= ~(1<<PB4);
		// CPOL = 0, CPHA = 0
		SPCR &= ~(1<<CPOL); // idle low for SCK
		SPCR &= ~(1<<CPHA); // clock edge for MOSI data shift = high to low
		SPCR &= ~(1<<DORD); // MSB first

	}
	else if(spiSensorId == FLAG_SPI_QDEC_R)
	{
		PORTB &= ~(1<<PB0);
		// CPOL = 0, CPHA = 0
		SPCR &= ~(1<<CPOL); // idle low for SCK
		SPCR &= ~(1<<CPHA); // clock edge for MOSI data shift = high to low
		SPCR &= ~(1<<DORD); // MSB first
	}
	else if(spiSensorId == FLAG_SPI_ACC)
	{
		PORTB &= ~(1<<PB3);
		// CPOL = 1, CPHA = 1
		SPCR |= (1<<CPOL);
		SPCR |= (1<<CPHA);
		SPCR &= ~(1<<DORD); // MSB first

	}
	else if(spiSensorId == FLAG_SPI_NONE)
	{
		PORTB |= (1<<PB3) | (1<<PB4) | (1<<PB0);
	}

	/* CODE END */
}

/* Sende Daten an den aktuell aktiven Slave und lese mepfangene Daten sofort aus */
uint8_t spi_sendAndRead(uint8_t data) {
	/* CODE START */
	uint8_t receivedData = 0;

	// Damit SCL läuft, muss etwas in SPDR geschrieben werden
	// Warte bis die Übertragung abgeschlossen ist (SPIF überprüfen)
	// SPDR muss gelesen werden, um SPIF zu löschen

	SPDR = data;
	while(!(SPSR & (1<<SPIF)));
	//_delay_ms(1);
	receivedData = SPDR;
	return receivedData;

	/* CODE END */
}

/* Initialisierung des Beschleunigungssensors */
void acc_init(void)
{	
	
	// Konfiguriere den Beschleunigungssensor
	acc_writeRegister(ADXL_BW_RATE,		0b00001010 ); // Output rate 100 Hz, Bandwidth 50 Hz
	acc_writeRegister(ADXL_DATA_FORMAT, 0b00001011 ); // Volle Aufl�sung, Range +-16g !!
		
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

	// SS leitung des Beschleunigungssensors aktivieren
	// Daten senden
	// SS leitung des Beschleunigungssensors deaktivieren
	spi_select(FLAG_SPI_ACC);
	spi_sendAndRead(registerByte);
	spi_sendAndRead(data);
	spi_select(FLAG_SPI_NONE);

	/* CODE END */
}

/* Auslesen des Beschleunigungssensors
	OUTPUT: - int accData[3]: Messwerte des Sensors in x (accData[0]), y (accData[1]), z (accData[2]) 
	TIPP zum Auslesen: vgl. 'ADXL345 Quick start guide.pdf', Seite 4, Fig 7 */
void acc_getData(int16_t *accData)
{
	/* CODE START */

	// CPOL = 1, CPHA = 1
	SPCR |= (1<<CPOL);
	SPCR |= (1<<CPHA);
	
	// X, Y, Z können gleichzeitig ausgelesen werden durch senden von 0xF2
	// Daten zusammensetzen

	// SS leitung des Beschleunigungssensors aktivieren
	// Daten senden
	// Daten empfangen
	// SS leitung des Beschleunigungssensors deaktivieren

	spi_select(FLAG_SPI_ACC);
	spi_sendAndRead(0xF2);
	accData[0] = spi_sendAndRead(0);
	accData[0] |= spi_sendAndRead(0) << 8;
	accData[1] = spi_sendAndRead(0);
	accData[1] |= spi_sendAndRead(0) << 8;
	accData[2] = spi_sendAndRead(0);
	accData[2] |= spi_sendAndRead(0) << 8;
	spi_select(FLAG_SPI_NONE);


	/* CODE END */
}

/* CODE START */
/* CODE END */