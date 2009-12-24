/** @file shutter.h
 *
 * Shutter active object. Handles the cameras shutter. Implements different
 * behaviour for the different shutter modes.
 *
 * The shutter time is measured using busy loops to get accurate timing.
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
#include "debug.h"
#include "param.h"

#include <util/delay.h>

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

#define SHUTTER_INIT()	DDRB |= _BV(0)
#define SHUTTER_ON()	PORTB |= _BV(0)
#define SHUTTER_OFF()	PORTB &= ~_BV(0)

/**
 * Triggers the shutter.
 * @param us Shut time in us
 */
void shutter_trigger(uint32_t us)
{
	double ms;

	// Compensate for call to _delay_ms()
	us -= 20;
	ms = (us / 1000.0);

	// _delay_ms() can handle delays up to 6 seconds
	if (ms < 6000) {
		DBG_LED_ON();
		SHUTTER_ON();
		_delay_ms(ms);
		SHUTTER_OFF();
		DBG_LED_OFF();
	} else {
		DBG_LED_ON();
		SHUTTER_ON();
		while (ms >= 1000.0) {
			_delay_ms(1000.0);
			ms -= 1000.0;
		}
		_delay_ms(ms);
		SHUTTER_OFF();
		DBG_LED_OFF();
	}
}


/**
 * Constructor.
 */
void shutter_ctor(void)
{
	// Set shutter pin
	SHUTTER_INIT();

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
		//shutter_trigger(shutter_time[pd.shutter_time].us);
		shutter_trigger(200000);
		QActive_post((QActive *) &prog_ao, SIG_SHUTTER_DONE, 0);
		return Q_HANDLED();
	case SIG_SHUTTER_STOP:
		return Q_HANDLED();
	}

	return Q_SUPER(&QHsm_top);
}


