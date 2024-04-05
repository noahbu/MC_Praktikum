#ifndef SENSORENSPI_H_
#define SENSORENSPI_H_

#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <util/twi.h>

#include "routines/uart.h"

/* Makros für den Quadratur-Decoder */
// Count modes
#define NQUAD 0x00 //non-quadrature mode
#define QUADRX1 0x01 //X1 quadrature mode
#define QUADRX2 0x02 //X2 quadrature mode
#define QUADRX4 0x03 //X4 quadrature mode

// Running modes
#define FREE_RUN 0x00
#define SINGE_CYCLE 0x04
#define RANGE_LIMIT 0x08
#define MODULO_N 0x0C

// Index modes
#define DISABLE_INDX 0x00 //index_disabled
#define INDX_LOADC 0x10 //index_load_CNTR
#define INDX_RESETC 0x20 //index_rest_CNTR
#define INDX_LOADO 0x30 //index_load_OL
#define ASYNCH_INDX 0x00 //asynchronous index
#define SYNCH_INDX 0x80 //synchronous index

// Clock filter modes
#define FILTER_1 0x00 //filter clock frequncy division factor 1
#define FILTER_2 0x80 //filter clock frequncy division factor 2

// Flag modes
#define NO_FLAGS 0x00 //all flags disabled
#define IDX_FLAG 0x10 //IDX flag
#define CMP_FLAG 0x20 //CMP flag
#define BW_FLAG 0x40 //BW flag
#define CY_FLAG 0x80 //CY flag

// 1 to 4 bytes data-width
#define BYTE_4 0x00 //four byte mode
#define BYTE_3 0x01 //three byte mode
#define BYTE_2 0x02 //two byte mode
#define BYTE_1 0x03 //one byte mode

// Enable/disable counter
#define EN_CNTR 0x00 //counting enabled
#define DIS_CNTR 0x04 //counting disabled

// LS7366R op-code list
#define CLR_MDR0 0x08
#define CLR_MDR1 0x10
#define CLR_CNTR 0x20
#define CLR_STR 0x30
#define READ_MDR0 0x48
#define READ_MDR1 0x50
#define READ_CNTR 0x60
#define READ_OTR 0x68
#define READ_STR 0x70
#define WRITE_MDR1 0x90
#define WRITE_MDR0 0x88
#define WRITE_DTR 0x98
#define LOAD_CNTR 0xE0
#define LOAD_OTR 0xE4
/* ENDE (Makros für den Quadratur-Decoder) */


/* Makros für den Beschleunigungssensor */
// Register des Beschleunigungssensors
#define ADXL_DEVID			0x00
#define ADXL_THRESH_TAP		0x1D
#define ADXL_OFSX			0x1E
#define ADXL_OFSY			0x1F
#define ADXL_OFSZ			0x20
#define ADXL_DUR			0x21
#define ADXL_LATENT			0x22
#define ADXL_WINDOW			0x23
#define ADXL_THRESH_ACT		0x24
#define ADXL_THRESH_INACT	0x25
#define ADXL_TIME_INACT		0x26
#define ADXL_ACT_INACT_CTL	0x27
#define ADXL_THRESH_FF		0x28
#define ADXL_TIME_FF		0x29
#define ADXL_TAP_AXIS		0x2A
#define ADXL_ACT_TAP_STATUS	0x2B
#define ADXL_BW_RATE		0x2C
#define ADXL_POWER_CTL		0x2D
#define ADXL_INT_ENABLE		0x2E
#define ADXL_INT_MAP		0x2F
#define ADXL_INT_SOURCE		0x30
#define ADXL_DATA_FORMAT	0x31
#define ADXL_DATAX0			0x32
#define ADXL_DATAX1			0x33
#define ADXL_DATAY0			0x34
#define ADXL_DATAY1			0x35
#define ADXL_DATAZ0			0x36
#define ADXL_DATAZ1			0x37
#define ADXL_FIFO_CTL		0x38
#define ADXL_FIFO_STATUS	0x39
/* ENDE (Makros für den Beschleunigungssensor) */

/* Makros zur einfachen Auswertung der Slaves am SPI Bus */
#define FLAG_SPI_QDEC_L 0
#define FLAG_SPI_QDEC_R 1
#define FLAG_SPI_ACC 2
#define FLAG_SPI_NONE 99

/* Initialisierungsfunktionen für SPI, Quadraturdecoder und Accelerometer */
void spi_init(void);
void qdec_init(void);
void acc_init(void);

/* SPI Funktionen */
void spi_select(uint8_t side);
uint8_t spi_sendAndRead(uint8_t data);

/* Decoder Funktionen */
void qdec_writeRegister(uint8_t spiSensorId, uint8_t registerByte, uint8_t data);
void qdec_writeCommand(uint8_t spiSensorId, uint8_t command);
int32_t qdec_getCounts(uint8_t spiSensorId);

/* Beschleunigungssensoren Funktionen */
void acc_writeRegister(uint8_t registerByte, uint8_t data);
void acc_getData(int16_t *accData);
/* CODE START */
/* CODE END */

#endif /* SENSORENSPI_H_ */