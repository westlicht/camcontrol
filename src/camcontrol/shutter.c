/**
 * Shutter control using 8-bit timer (timer2).
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "defines.h"
#include "shutter.h"

#define TIMER_INTERVAL	4		/**< Timer interval in us */

#define SHUTTER_ON()	PORTA |= _BV(6)
#define SHUTTER_OFF()	PORTA &= ~_BV(6)


static uint32_t counter;

ISR(TIMER2_COMP_vect)
{
	counter -= OCR2;
	if (counter == 0) {
		SHUTTER_OFF();
		TIMSK &= ~_BV(OCIE2);
	}

	OCR2 = counter <= 0xff ? counter : 0xff;
}

/**
 * Initializes the shutter.
 */
void shutter_init(void)
{
	// Set Timer2 in CTC mode, 1/64 prescaler, this gives a timer resolution of 4us
	TCCR2 = ((1 << WGM21) | (0 << WGM20) | (3 << CS20));
}

/**
 * Triggers the shutter.
 * @param us Shut time in us
 */
void shutter_trigger(uint32_t us)
{
	TIMSK &= ~_BV(OCIE2);

	counter = us / TIMER_INTERVAL;

	SHUTTER_ON();

	// Setup timer and enable interrupt
	OCR2 = counter <= 0xff ? counter : 0xff;
	TCNT2 = 0;
	TIMSK |= _BV(OCIE2);
}
