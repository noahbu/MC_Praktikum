#include <avr/io.h>
#include <util/delay.h>

#define Arot PD5
#define Agelb PD6
#define Agrun PD7

#define Brot PB2
#define Bgelb PB1
#define Bgrun PB0

#define Crot PC2
#define Cgelb PC1
#define Cgrun PC0

#define Drot PC3
#define Dgelb PC4
#define Dgrun PC5

// Function to introduce delay and check button press
// If the button is pressed, the mode changes and the function returns 1, otherwise returns 0
void mydelay_ms(uint16_t delay_ms, uint8_t *tag) {
	uint16_t i;
	static uint8_t buttonPreviouslyPressed = 0; // Keep track of the button's previous state

	for (i = 0; i < delay_ms; i += 50) { // Check every 50 ms
		_delay_ms(50); // Wait for 50 ms

		if (PIND & (1<<PD2)) { // Check if the button is currently pressed
			if (!buttonPreviouslyPressed) { // If button was not pressed before (edge detection)
				_delay_ms(50); // Debounce delay
				if (PIND & (1<<PD2)) { // Confirm the button is still pressed
					*tag = !*tag; // Toggle 'tag'
					buttonPreviouslyPressed = 1; // Update the state to indicate the button is now pressed
				}
			}
			} else {
			buttonPreviouslyPressed = 0; // Button is not pressed, reset the state
		}
	}
}



int main(void) {
	// Set DDR (Data Direction Register) for all traffic lights
	DDRD |= (1<<Arot) | (1<<Agelb) | (1<<Agrun);
	DDRB |= (1<<Brot) | (1<<Bgelb) | (1<<Bgrun);
	DDRC |= (1<<Crot) | (1<<Cgelb) | (1<<Cgrun) | (1<<Drot) | (1<<Dgelb) | (1<<Dgrun);

	uint8_t tag = 1; // Start in day mode

	while (1) {
		if(tag == 1) { // Day mode logic
			// A, C is green; B, D is red
			PORTD |= (1<<Agrun);
			PORTC |= (1<<Cgrun) | (1<<Drot);
			PORTB |= (1<<Brot);

			mydelay_ms(800, &tag);

			// Transition from green to yellow for A and C
			PORTD |= (1<<Agelb);
			PORTC |= (1<<Cgelb) | (1<<Dgelb);
			PORTB |= (1<<Bgelb);

			mydelay_ms(300, &tag);

			// Turn off green and yellow lights for A and C
			PORTD &= ~((1<<Agrun) | (1<<Agelb));
			PORTC &= ~((1<<Cgrun) | (1<<Cgelb) | (1<<Drot) | (1<<Dgelb));
			PORTB &= ~((1<<Brot) | (1<<Bgelb));

			// A, C is red; B, D is green
			PORTD |= (1<<Arot);
			PORTC |= (1<<Crot) | (1<<Dgrun);
			PORTB |= (1<<Bgrun);

			mydelay_ms(800, &tag);
			// Transition from green to yellow for B and D
			PORTD |= (1<<Agelb);
			PORTC |= (1<<Cgelb) | (1<<Dgelb);
			PORTB |= (1<<Bgelb);

			mydelay_ms(300, &tag);

			// Turn off all lights before the next cycle
			PORTD &= ~((1<<Arot) | (1<<Agelb));
			PORTC &= ~((1<<Crot) | (1<<Cgelb) | (1<<Dgrun) | (1<<Dgelb));
			PORTB &= ~((1<<Bgrun) | (1<<Bgelb));

			} 
			else if( tag == 0)
			{ // Night mode logic
			// Toggle yellow lights
			PORTB ^= (1<<Bgelb);
			PORTC ^= (1<<Dgelb) | (1<<Cgelb);
			PORTD ^= (1<<Agelb);

			mydelay_ms(800, &tag); // Check for mode change
		}
	}

	return 0;
}
