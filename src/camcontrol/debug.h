
#ifndef __DEBUG_H__
#define __DEBUG_H__

#include <avr/io.h>
#include "lcd.h"

#define DBG_LED_ON()	PORTA |= _BV(6)
#define DBG_LED_OFF()	PORTA &= ~_BV(6)

#endif // __DEBUG_H__
