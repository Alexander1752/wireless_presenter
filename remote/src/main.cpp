#include <VirtualWire.h>

#include "usart.h"
#include "debug.h"

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#define DEBOUNCE_DELAY 50

volatile char prev_pressed = 0, next_pressed = 0;

ISR(INT0_vect) {
    next_pressed = 1;
}

ISR(INT1_vect) {
    prev_pressed = 1;
}

void setup(){
    if (DEBUG) {
        USART0_init(CALC_USART_UBRR(9600));
        USART0_use_stdio();
    }

    vw_setup(2000);	 // Bits per sec
    vw_set_tx_pin(A3);
    DDRC |= (1 << PC3); // A3 as output

    DDRD &= ~(1 << PD2 | 1 << PD3 | 1 << PD4 | 1 << PD5 | 1 << PD6); // button pins as input
    PORTD |= (1 << PD2 | 1 << PD3 | 1 << PD4 | 1 << PD5 | 1 << PD6); // activate pull-up resistors

    EIMSK |= (1 << INT0 | 1 << INT1);
    EICRA = (1 << ISC00 | 1 << ISC01 | 1 << ISC10 | 1 << ISC11); // activate on rising edge (when button released)

    sei();
}

unsigned long lastDebouncePrev = 0, lastDebounceNext = 0;
unsigned long current_millis;

void loop() {
    if (next_pressed) {
        next_pressed = 0;
        current_millis = millis();
        if (current_millis - lastDebounceNext > DEBOUNCE_DELAY) {
            vw_send((uint8_t *)"r", 1);
            vw_wait_tx(); // Wait until the whole message is gone
            dprintf("%c\n", 'r');
        }

        lastDebounceNext = current_millis;
    }

    if (prev_pressed) {
        prev_pressed = 0;
        current_millis = millis();
        if (current_millis - lastDebouncePrev > DEBOUNCE_DELAY) {
            vw_send((uint8_t *)"l", 1);
            vw_wait_tx(); // Wait until the whole message is gone
            dprintf("%c\n", 'l');
        }

        lastDebouncePrev = current_millis;
    }
}