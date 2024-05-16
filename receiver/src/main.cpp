#include <VirtualWire.h>
#include "usart.h"

void setup() {
    USART0_init(CALC_USART_UBRR(9600));
    USART0_use_stdio();

    vw_setup(2000);   // Bits per sec
    vw_rx_start();    // Start the receiver PLL running

    DDRB |= (1 << PB5);   // pin 13 as output to onboard LED
    PORTB &= ~(1 << PB5); // turn LED off
}

void loop() {
    uint8_t buf[VW_MAX_MESSAGE_LEN];
    uint8_t buflen = VW_MAX_MESSAGE_LEN;

    if (vw_get_message(buf, &buflen)) { // Non-blocking
        PORTB &= ~(1 << PB5); // flash onboard LED to show received good message
        printf("%c\n", buf[0]);
        PORTB &= ~(1 << PB5); // turn LED off
    }
}