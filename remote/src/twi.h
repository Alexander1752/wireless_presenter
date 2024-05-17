/* twi.h - TWI library for ATMega324P */

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <util/twi.h>
#include <stdio.h>
#include "debug.h"
// #include "usart.h"

/**
 * According to ATMega324P/V Datasheet Section 23.5.2, the formula used to get the SCL
 * frequency is:
 * SCL_frequency = CPU_Clock_Frequency / (16 + 2 * TWBR * PrescalerValue)
*/
#define F_SCL 100000UL
#define PRESCALER 1
#define TWBR_VAL ((((F_CPU / F_SCL) / PRESCALER) - 16 ) / (2 * PRESCALER))

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Initialize TWI by setting the bitrate
*/
void twi_init(void);

/**
 * \brief Transmit START condition
*/
void twi_start(void);

/**
 * \brief Write a byte of data
*/
void twi_write(int8_t data);

/**
 * \brief Read a byte of data with ACK enabled
*/
void twi_read_ack(int8_t *data);

/**
 * \brief Read a byte of data without ACK
*/
void twi_read_nack(int8_t *data);

/**
 * \brief Transmit STOP condition
*/
void twi_stop(void);

/**
 * \brief Search for slave devices and print them to serial
*/
void twi_discover(void);

#ifdef __cplusplus
}
#endif