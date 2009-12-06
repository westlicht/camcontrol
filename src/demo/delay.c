#include "delay.h"

// very simple delay
// CPU oscilator is 16MHz
// 1 instr = 0.0625 uS
// 1 cycle = 4 instruction
// 1 us = 4 cycle

#define DFACTOR 	4

static volatile unsigned long i;


void delay_us(unsigned long d)
{
	i = DFACTOR * d;
	while (--i != 0);
}

void delay_ms(unsigned long d)
{
	i = DFACTOR * d * 1000;
	while (--i != 0);
}
