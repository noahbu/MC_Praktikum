/*
 * MidTermTest.c
 *
 * Author : Klaus Albert and Johannes Strohm
 */ 

/* Include standard header files from Atmel */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>

/* Include delay lib from Atmel */
#include <util/delay.h>

/* Include custom UART lib for serial communication */
#include "uart.h"

/* Predefined constants */
#define MAX_TIMER_INT_COUNT 31
#define ADC_PITCH 1.0f/512.0f
#define ADC_OFFSET 512

/* Function prototypes */
void io_init(void);
void adc_init(void);
void timer_init(void);
uint16_t adc_read(uint8_t axisIndex);
uint8_t button_read(void);
void led_set(uint8_t buttonState);
float adc_convert(uint16_t adcValue);
void read_and_send(void);

volatile uint8_t timerIntCount = 0;

/* Init main function */
int main(void)
{
	/* Call init functions to configure the uC */
	io_init();
	adc_init();
	timer_init();
	uart_init();
	
	/* Global interrupt activation */
	sei();
	
    /* Main loop */
    while (1) 
    {
		/* Check the max count of timer interrupt */
		if(timerIntCount >= MAX_TIMER_INT_COUNT)
		{
			/* Execute tasks */
						
			/* Run read and send */	
			read_and_send();
						
			/* Reset counter variable */
			timerIntCount = 0;			
		}
    }
}

void read_and_send(void)
{
	/* Define local variables */
	char buff[50];	
	uint8_t state;
	uint16_t adcXValue;
	uint16_t adcYValue;
	float xValue;
	float yValue;
	
	/* Read button and set LEDs */
	state = button_read();
	led_set(state);
	
	/* Read ADC values */
	adcXValue = adc_read(0);
	adcYValue = adc_read(1);
	
	/* Convert values to scaled float values */
	xValue = adc_convert(adcXValue);
	yValue = adc_convert(adcYValue);
	
	/* Generate output string */
	sprintf(buff,"AX %d AY %d FX: %1.2f FY: %1.2f B: %d\r\n",adcXValue,adcYValue,xValue,yValue,state);
		
	/* Send string */
	uart_puts((uint8_t *)buff);
}

/* Interrupt service routine for timer 2 compare match */
ISR(TIMER2_COMP_vect)
{
	/* Increment timer count variable */
	timerIntCount++;
}

/* Initialization for IO ports (LEDS, buttons etc.) */
void io_init(void)
{
	DDRD |= (1 << PD5); // PD5 as output for red LED
	DDRD |= (1 << PD6); // PD6 as output yellow LED
	DDRD |= (1 << PD7); // PD7 as output green LED
}

/* Initialization for ADC */
void adc_init(void)
{
    // ADC initialisieren
    // ADC Prescaler auf 128 setzen für 125 kHz (16mhz / 128 = 125kHz)

    ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);

    // ADC aktivieren
    ADCSRA |= (1 << ADEN);
    // Referenzspannung auf AVCC setzen und clear ADLAR for 10-bit results
    ADMUX = (1 << REFS0);

    // No need to set MUX here as it's done in adc_read before each conversion

    // erste Wandlung durchführen, damit ADC startklar ist (Warmup)
    ADCSRA |= (1 << ADSC);
    while(ADCSRA & (1 << ADSC)); // Wait for conversion to finish
}

/* Initialization for timer */
void timer_init(void)
{
	// alle 16 ms soll die ISR aufgerufen werden
	// dies soll insgesamt dann zu einer ABtastzeit von des ADC von 500ms führen, da in der ISR ein counter auf 31 hochzählt
	// 16ms * 31 = 496ms -> 500ms

	// Timer 2 konfigurieren
	// CTC Modus
	TCCR2 |= (1 << WGM21);
	// Prescaler auf 1024 setzen
	TCCR2 |= (1 << CS22) | (1 << CS21) | (1 << CS20);
	// Vergleichswert setzen
	OCR2 = 249;
	// Vergleichsinterrupt aktivieren
	TIMSK |= (1 << OCIE2);
	
}

/* Read joystick ADC (axisIndex = 0 for ADC6 or axisIndex = 1 for ADC7) */
uint16_t adc_read(uint8_t axisIndex)
{    
    if(axisIndex == 1)
    {
        // Auswahl von Kanal 7 (Joystick Y)
        ADMUX =   | (1<<MUX2) | (1<<MUX1); // Clear ADLAR and select channel
    }
    else if (axisIndex == 0)
    {
        // Auswahl von Kanal 6 (Joystick X)
        ADMUX = (ADMUX & 0xF0) | (1<<MUX0) | (1<<MUX1) | (1<<MUX2); // Clear ADLAR and select channel
    }
    else
	{
		// Invalid axis index
		return 100;
	}

    // ADC starten
    ADCSRA |= (1 << ADSC);
    while(ADCSRA & (1 << ADSC)); // Wait for conversion to finish

    // Read 10-bit result correctly
    uint16_t result = ADCL;
    result |= (uint16_t)ADCH << 8;

    return result;
}

/* Scale ADC result to approx -1<= scaled-adc-result <=+1  */
float adc_convert(uint16_t adcValue)
{
	// funktion soll den ADC Wert auf den Bereich -1.0f bis +1.0f skalieren
	// ADC_OFFSET = 512
	// ADC_PITCH = 1.0f/512.0f
	// ADC_VALUE = 0-1023
	// ADC_VALUE - ADC_OFFSET = -512 - +511
	// (ADC_VALUE - ADC_OFFSET) * ADC_PITCH = -1.0f - +1.0f
	return (float)(adcValue - ADC_OFFSET) * ADC_PITCH;
	//return adcValue;
	//return (float)adcValue - (float)ADC_OFFSET * ADC_PITCH;

}

/* Checks joystick button, return 1 if pressed, 0 if released */
uint8_t button_read(void)
{
	// Button is connected to PD4 with a pull-up resistor, meaning it is high when the button is not pressed
	if(PIND & (1 << PD2))
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

/* Sets LEDs depended on state and toggle yellow LED */
void led_set(uint8_t state)
{
	PORTD ^= (1<<PD6);
	//wenn state == 1, dann soll die grüne LED leuchten
	//wenn state == 0, dann soll die rote LED leuchten
	//bei jedem aufruf soll die gelbe LED toggeln
	if(state == 1)
	{
		PORTD |= (1 << PD7);
		PORTD &= ~(1 << PD5);
	}
	else
	{
		PORTD |= (1 << PD5);
		PORTD &= ~(1 << PD7);
	}
}

