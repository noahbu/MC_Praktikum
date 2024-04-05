#include <avr/io.h>
#include <avr/interrupt.h>
#include "uart.h"
#include <util/delay.h>
#include <math.h>

void initTimer1ForToneGeneration();


volatile int mod_step = 0; // Current step in modulation
volatile int mod_direction = 1; // Direction of frequency change (1 = up, -1 = down)
const int mod_depth = 10; // How much to adjust OCR1A for modulation
const int mod_max_step = 200; // Number of steps before changing direction
uint16_t base_OCR1A = 0; // Base value of OCR1A for vibrato modulation
uint8_t vibrato_enabled = 0; // Flag to enable/disable vibrato
uint8_t v_high = 1; // Flag to indicate high or low modulation

void play_tone(uint16_t frequency) {
    // Ensure the timer is started with the prescaler set to 8
    TCCR1B |= (1 << CS11);

    // Calculate and set OCR1A based on the frequency
    OCR1A = (16000000 / (2 * 8 * frequency)) - 1;

    // Store the current OCR1A as the base for vibrato modulation
    base_OCR1A = OCR1A;
}




int main(void) {
    // Initialize Timer1 to generate a 440Hz tone
    initTimer1ForToneGeneration();

    // Initialize UART
    // Wert den wir empfangen wollen via UART
    uint8_t rec;
    uart_init();

    // Configure PB2 (connected to the buzzer) as an output
    DDRB |= (1 << PB2);

    // Enable global interrupts
    sei();

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
            case 'v':
                vibrato_enabled = !vibrato_enabled;
                break;

            default:
                // Optional: Handle any other invalid input if necessary
                break;
        }



    }
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


ISR(TIMER1_COMPA_vect) {
    static const uint16_t vibrato_range = 210; // Range of vibrato effect
    static const uint8_t vibrato_step = 10; // Step size for vibrato modulation

    // Toggle the buzzer pin to generate the tone
    PORTB ^= (1 << PB2);

    if (vibrato_enabled) {
        if (v_high) {
            // Increase OCR1A to simulate vibrato effect
            if (OCR1A < base_OCR1A + vibrato_range) {
                OCR1A += vibrato_step;
            } else {
                v_high = 0; // Switch direction to decrease frequency
            }
        } else {
            // Decrease OCR1A to simulate vibrato effect
            if (OCR1A > base_OCR1A - vibrato_range) {
                OCR1A -= vibrato_step;
            } else {
                v_high = 1; // Switch direction to increase frequency
            }
        }
    }
}
