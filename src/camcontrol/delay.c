
#include "defines.h"

#include <util/delay.h>


void delay_us(unsigned long us)
{
	_delay_us(us);
}

void delay_ms(unsigned long ms)
{
	_delay_ms(ms);
}
