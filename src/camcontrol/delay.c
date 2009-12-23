/** @file delay.c
 *
 * Delay functions.
 *
 * @author Simon Kallweit, simon@weirdsoft.ch
 */

#include "defines.h"
#include <util/delay.h>

/**
 * Delays execution in a busy loop.
 * @param us Microseconds
 */
void delay_us(unsigned long us)
{
	_delay_us(us);
}

/**
 * Delays execution in a busy loop.
 * @param ms Milliseconds
 */
void delay_ms(unsigned long ms)
{
	_delay_ms(ms);
}
