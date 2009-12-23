/** @file debug.h
 *
 * Debug support.
 *
 * @author Simon Kallweit, simon@weirdsoft.ch
 */

#ifndef __DEBUG_H__
#define __DEBUG_H__

#include <avr/io.h>
#include <stdio.h>
#include "lcd.h"

/** Set for debugging output */
#define DEBUG

/* Debug LED */
#define DBG_LED_ON()	PORTA |= _BV(6)
#define DBG_LED_OFF()	PORTA &= ~_BV(6)

/* Debug print */
#ifdef DEBUG
# define DBG(_fmt_, _args_...) printf(_fmt_, ##_args_)
#else
# define DBG(_fmt_, _args_...)
#endif

void on_assert(const char *filename, uint16_t line);

/** Assertion macro */
#define ASSERT(_expr_)							\
	do {										\
		if (!(_expr_)) {						\
			on_assert(__FILE__, __LINE__);		\
		}										\
	} while(0)

#endif // __DEBUG_H__
