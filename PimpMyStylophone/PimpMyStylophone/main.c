#include <avr/io.h>
#include <avr/interrupt.h>
#include "uart.h"
#include <util/delay.h>
#include <math.h>

void initTimer1ForToneGeneration();
void initADC();

volatile uint8_t D_init = 0; // Initial potentiometer position, set during ADC initialization


void play_tone(uint16_t frequency) {
    uint16_t OCnom; // Corrected type to uint16_t

    // Start timer with prescaler set to 8
    TCCR1B |= (1 << CS11);

    // Calculate OCR1A based on the input frequency
    OCnom = (16000000 / (2 * 8 * frequency)) - 1;
    
    // Start ADC conversion and wait for it to finish
    ADCSRA |= (1 << ADSC);
    while(ADCSRA & (1 << ADSC));

    uint8_t adc_result = ADCH;

    // Calculate the difference from the initial position
    int D_potentiometer = adc_result - D_init; // This can be negative

    // Adjust OCR1A based on the potentiometer's difference
    // Using a floating-point operation to keep precision and then casting the result back to integer
    OCR1A = OCnom + (int)((900.0 * D_potentiometer) / 255.0);
}




int main(void) {

    // Enable global interrupts
    sei();
    // Initialize Timer1 to generate a 440Hz tone
    initTimer1ForToneGeneration();
    initADC();

    //uint8_t OCnom;

    // Initialize UART
    // Wert den wir empfangen wollen via UART
    uint8_t rec;
    uart_init();

    // Configure PB2 (connected to the buzzer) as an output
    DDRB |= (1 << PB2);

    while(1) {
        // zeichen von UART lesen
        rec = uart_getch();
    

        switch(rec) {
            case 'a': play_tone(226); break;
            case 'w': play_tone(277); break;
            case 's': play_tone(294); break;
            case 'e': play_tone(311); break;
            case 'd': play_tone(330); break;
            case 'f': play_tone(349); break;
            case 't': play_tone(370); break;
            case 'g': play_tone(392); break;
            case 'z': play_tone(415); break;
            case 'h': play_tone(440); break;
            case 'u': play_tone(466); break;
            case 'j': play_tone(494); break;
            case 'k': play_tone(523); break;
            case ' ': // Blank space sent when key is lifted
                // Silence: Stop the timer and ensure the pin is low
                TCCR1B &= ~(1 << CS11); // Stop the timer
                PORTB &= ~(1 << PB2); // Make sure the output pin is low
                break;
            default:
                // Optional: Handle any other invalid input if necessary
                break;
        }

    }
}

ISR(TIMER1_COMPA_vect) {

    // Toggle the buzzer pin to generate the tone
    PORTB ^= (1 << PB2);

}

void initTimer1ForToneGeneration() {
    // Set Timer1 to CTC mode
    TCCR1B |= (1 << WGM12);

    // Calculate and set OCR1A for a 440Hz tone
    // Assuming a prescaler of 8 and a 16MHz clock, the formula from the Atmega datasheet is:
    // OCR1A = F_CPU / (2 * prescaler * frequency) - 1
    OCR1A = (16000000 / (2 * 8 * 523)) - 1;//(16000000 / (2 * 8 * 440)) - 1;

    // Set prescaler to 8
    TCCR1B |= (1 << CS11);

    // Enable the output compare match interrupt for Timer1 A
    TIMSK |= (1 << OCIE1A);
}

void initADC(){
    // ADC initialisieren
    // ADC Prescaler auf 128 setzen
    ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
    // ADC aktivieren
    ADCSRA |= (1 << ADEN);
    // Referenzspannung auf AVCC setzen
    ADMUX |= (1 << REFS0) | (1 << ADLAR);
	
	//Auswahl von Kanal 7 (Fotodiode
	//ADMUX |= (1<<MUX0)|(1<<MUX1)|(1<<MUX2);
    ADMUX |= (1<<MUX2)|(1<<MUX1);

    // erste Wandlung durchführen, damit ADC startklar ist (Warmup)
    ADCSRA |= (1 << ADSC);
    while(ADCSRA & (1 << ADSC));

    //wandlungsergebnis abfragen
    D_init = ADCH;
}





