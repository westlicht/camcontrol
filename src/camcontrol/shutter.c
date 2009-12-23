/** @file shutter.h
 *
 * Shutter active object. Handles the cameras shutter. Implements different
 * behaviour for the different shutter modes.
 *
 * Internally uses the Timer2 to do the shutter timing.
 *
 * @author Simon Kallweit, simon@weirdsoft.ch
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "qpn_port.h"
#include "bsp.h"
#include "camcontrol.h"
#include "defines.h"
#include "shutter.h"
#include "prog.h"

/** Shutter active object structure */
struct shutter_ao {
	QActive super;
	uint32_t seconds;
};

static QState shutter_initial(struct shutter_ao *me);
static QState shutter_idle(struct shutter_ao *me);

/** Shutter active object */
struct shutter_ao shutter_ao;

enum timeouts {
	TIMEOUT_SECOND = TICKS(1000),
};


#define TIMER_INTERVAL	4		/**< Timer interval in us */

#define SHUTTER_ON()	PORTA |= _BV(6)
#define SHUTTER_OFF()	PORTA &= ~_BV(6)


static uint32_t counter;

/**
 * Timer2 interrupt.
 */
ISR(TIMER2_COMP_vect)
{
	counter -= OCR2;
	if (counter == 0) {
		SHUTTER_OFF();
		TIMSK &= ~_BV(OCIE2);
		QActive_postISR((QActive *) &shutter_ao, SIG_SHUTTER_DONE, 0);
	}

	OCR2 = counter <= 0xff ? counter : 0xff;
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


/**
 * Constructor.
 */
void shutter_ctor(void)
{
	// Set Timer2 in CTC mode, 1/64 prescaler, this gives a timer resolution of 4us
	TCCR2 = ((1 << WGM21) | (0 << WGM20) | (3 << CS20));

	QActive_ctor((QActive *) &shutter_ao, (QStateHandler) shutter_initial);
}

/**
 * Initial state.
 */
static QState shutter_initial(struct shutter_ao *me)
{
	return Q_TRAN(shutter_idle);
}

/**
 * Idle state.
 */
static QState shutter_idle(struct shutter_ao *me)
{
	switch (Q_SIG(me)) {
	case Q_ENTRY_SIG:
		return Q_HANDLED();
	case Q_EXIT_SIG:
		return Q_HANDLED();
	case Q_TIMEOUT_SIG:
		return Q_HANDLED();
	case SIG_SHUTTER_START:
		shutter_trigger(200000);
		QActive_post((QActive *) &prog_ao, SIG_SHUTTER_DONE, 0);
		return Q_HANDLED();
	case SIG_SHUTTER_STOP:
		return Q_HANDLED();
	}

	return Q_SUPER(&QHsm_top);
}


