/*
 * timer_blink.c
 *
 * Created: 03.04.2024 10:43:08
 * Author : delfi
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

uint8_t volatile count;
uint8_t volatile speed;



int main(void)
{
    
    //Anfangswerte der variable setzen
    count = 0;
    speed = 8;

    // Pin der roten und gelben LED an Ampel A als Ausgang konfigurieren
    DDRD |= (1 << PD5)| (1<<PD6); // PD5 als Ausgang konfigurieren


     // externen Interrupt INTO auf steigende flanke konfigurieren
    MCUCR |= (1<<ISC00) | (1<<ISC01);
	
    // externen Interrupt lokal aktivieren
    GICR |= (1<<INT0) | (1<<INT1);
    

    // Timer 0 konfigurieren (prescaler 1024)
    TCCR0 = (1 << CS02) | (1 << CS00); // Prescaler 1024

    // lokal aktivieren des timer 0 overflow interrupts
    TIMSK |= (1 << TOIE0);

    // globale Interrupts aktivieren
    sei();

    //rote LED an Ampel A einschalten
    PORTD |= (1 << PD5);

    while (1) 
    {
        //warten bis der timer 0 overflow interrupt aufgerufen wird
        if(count>=speed){
            //nach count overflow toggeln beider LEDs
            PORTD ^= (1 << PIND5);
            PORTD ^= (1 << PIND6);
            count = 0;
        }
    }
}


//timer 0 overflow interrupt service routine
ISR(TIMER0_OVF_vect){ 
    //count erhöhen
    count++;
}

//interrupt service routine für joystick
ISR(INT0_vect){
    // togglen der roten LED an Ampel A
    if (speed < 64){
        speed <<= 1;
    }

    //taster entprellen mit 500ms warten
    _delay_ms(500);
}

// interrupt service routine für taster 2
ISR(INT1_vect){
    // togglen der roten LED an Ampel A
    if (speed > 1){
        speed >>= 1;
    }

    //taster entprellen mit 500ms warten
    _delay_ms(500);
}



