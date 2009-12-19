
#ifndef __DEBUG_H__
#define __DEBUG_H__

#include <avr/io.h>
#include "lcd.h"

#define DBG_LED_ON()	PORTA |= _BV(6)
#define DBG_LED_OFF()	PORTA &= ~_BV(6)

void on_assert(const char *filename, uint16_t line);

#define ASSERT(_expr_)							\
	do {										\
		if (!(_expr_)) {							\
			on_assert(__FILE__, __LINE__);		\
		}										\
	} while(0)

#endif // __DEBUG_H__
