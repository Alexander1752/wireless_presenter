#include <VirtualWire.h>
#include <avr/sleep.h>
#include <avr/wdt.h>

#include "usart.h"
#include "debug.h"
#include "buttons.h"

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#define DEBOUNCE_DELAY 100

volatile uint32_t mills = 0;

uint8_t pinc = 0;
uint8_t pind = 0;

Button buttons[] ={
    Button('T'), // touch
    Button('c'), // click
    Button('r'), // next
    Button('l'), // prev
    Button('f'), // fullscreen
    Button('P'), // play
    Button('e'), // esc
};

uint32_t milliseconds() {
    return mills;
}

ISR(INT0_vect) {
    buttons[NEXT].pressed = 1;
}

ISR(INT1_vect) {
    buttons[PREV].pressed = 1;
}

ISR(PCINT1_vect) {
    pinc = PINC;

    if ((pinc & (1 << PC2)) == 0)
        buttons[CLICK].pressed = 1;

    if (pinc & (1 << PC0))
        buttons[TOUCH].pressed = 1;
}

ISR(PCINT2_vect) {
    pind = PIND;

    for (int i = PD4; i <= PD6; ++i) {
        if ((pind & (1 << i)) == 0)
            buttons[i].pressed = 1;
    }
}

ISR(TIMER2_COMPA_vect) {
    mills += 4;
}

void init_timer2() {
    TCCR2A |= (1 << WGM01); // enable count
    OCR2A = 0xFA;

    TCCR2B |= (1 << CS22) | (1 << CS21); // prescaler 256

    TIMSK2 |= (1 << OCIE2A);
}

void enter_power_save() {
#ifdef DEBUG
    if (DEBUG) {
        PRR |= (1 << PRUSART0);
        USART0_disable();
    }
#endif
    /////////// we have to yet see about PR-TWI
    PRR |= (1 << PRTIM1) | (1 << PRTWI);
    sleep_enable();
    sleep_bod_disable();
    sei();
    sleep_cpu();
    sleep_disable();

#ifdef DEBUG
    if (DEBUG) {
        PRR &= ~(1 << PRUSART0);
        USART0_enable();
    }
#endif

    PRR &= ~(1 << PRTIM1) | (1 << PRTWI);

    // cli();
}

void WDT_off(void)
{
    wdt_reset();
    /* Clear WDRF in MCUSR */
    MCUSR &= ~(1 << WDRF);
    /* Write logical one to WDCE and WDE */
    /* Keep old prescaler setting to prevent unintentional time-out
    */
    WDTCSR |= (1 << WDCE) | (1 << WDE);
    /* Turn off WDT */
    WDTCSR = 0x00;
}

void setup(){
#ifdef DEBUG
    if (DEBUG) {
        USART0_init(CALC_USART_UBRR(9600));
        USART0_use_stdio();
    }
#endif
    dprintf("Init started\n");

    vw_setup(2000);	   // Bits per sec
    vw_set_tx_pin(A3); // use A3 (PC3) pin

    // init top buttons pins
    DDRD &= ~(1 << PD2 | 1 << PD3 | 1 << PD4 | 1 << PD5 | 1 << PD6); // button pins as input
    PORTD |= (1 << PD2 | 1 << PD3 | 1 << PD4 | 1 << PD5 | 1 << PD6); // activate pull-up resistors

    // init prev/next external interrupt
    EIMSK |= (1 << INT0 | 1 << INT1);
    EICRA = (1 << ISC01 | 1 << ISC11); // activate on falling edge (when button pressed)

    // init top buttons pin change interrupt
    PCMSK2 = (1 << PCINT20) | (1 << PCINT21) | (1 << PCINT22); // enable change interrupt for button pins
    PCICR |= (1 << PCIE2); // enable pin change interrupt on port D

    // init bottom buttons and capacitive sensor pins
    DDRC &= ~(1 << PC0) | (1 << PC2); // button and sensor as input
    PORTC |= (1 << PC2); // activate pull-up resistor for button

    // init bottom button and capacitive sensor pin change interrupt
    PCMSK1 = (1 << PCINT10) | (1 << PCINT8);
    PCICR |= (1 << PCIE1); // enable pin change interrupt on port C

    // init laser and RF transmitter pins as output
    DDRC |= (1 << PC1) | (1 << PC3);

    init_timer2();

    WDT_off(); // disable watchdog

    set_sleep_mode(SLEEP_MODE_PWR_SAVE);
    ADCSRA = 0; // disable ADC
    PRR |= (1 << PRTIM0) | (1 << PRADC); // enable power save for timer0 and ADC

#ifndef DEBUG
    PRR |= (1 << PRUSART0);
#else
    if (!DEBUG)
        PRR |= (1 << PRUSART0);
#endif

    dprintf("Init complete\n");

    enter_power_save();
}

unsigned long lastDebouncePrev = 0, lastDebounceNext = 0;
unsigned long current_millis;

void loop() {
    current_millis = milliseconds();

    for (int i = 1; i <= 6; ++i) {
        if (buttons[i].pressed) {
            buttons[i].pressed = 0;
            if (current_millis - buttons[i].lastDebounce > DEBOUNCE_DELAY) {
                vw_send((uint8_t *)&(buttons[i].ch), 1);
                vw_wait_tx(); // Wait until the whole message is gone
                dprintf("\n%c\n\n", buttons[i].ch);
            }

            buttons[i].lastDebounce = current_millis;
        }
    }

    enter_power_save();
}