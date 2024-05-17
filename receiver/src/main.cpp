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

uint8_t buf[VW_MAX_MESSAGE_LEN + 1];
uint8_t buflen;

int16_t x, y, *ptr = (int16_t*)(buf + 1);

void loop() {
    buflen = VW_MAX_MESSAGE_LEN;

    if (vw_get_message(buf, &buflen)) { // Non-blocking
        PORTB &= ~(1 << PB5); // flash onboard LED to show received good message
        buf[buflen] = '\0';

        if (buf[0] == 'A') {
            x = ptr[0];
            y = ptr[1];
            sprintf((char*)buf, "A %hd %hd", x, y);
        }

        printf("%s\n", buf);
        PORTB &= ~(1 << PB5); // turn LED off
    }
}