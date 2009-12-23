/** @file defines.h
 *
 * Global definitions. Adapted from avr-lib stdiodemo.
 *
 * @author Simon Kallweit, simon@weirdsoft.ch
 * @author Joerg Wunsch
 */

#ifndef __DEFINES_H__
#define __DEFINES_H__

/* CPU frequency */
#define F_CPU 16000000UL

/* UART baud rate */
#define UART_BAUD  9600

/* HD44780 LCD port connections */
#define HD44780_PORT C
#define HD44780_RS PORT0
#define HD44780_RW PORT1
#define HD44780_E  PORT2
/* The data bits have to be in ascending order. */
#define HD44780_D4 PORT4
#define HD44780_D5 PORT5
#define HD44780_D6 PORT6
#define HD44780_D7 PORT7

#endif // __DEFINES_H__
