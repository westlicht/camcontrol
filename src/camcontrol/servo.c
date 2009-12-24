/** @file servo.c
 *
 * Servo active object. Handles movement of the servos.
 *
 * Internally uses the Timer1 to generate PWM signals for the two servos.
 *
 * @author Simon Kallweit, simon@weirdsoft.ch
 */

#include <avr/io.h>
#include <math.h>
#include "qpn_port.h"
#include "bsp.h"
#include "camcontrol.h"
#include "defines.h"
#include "prog.h"
#include "debug.h"
#include "servo.h"

#define SERVO_RANGE_X	700		/**< Horizontal servo range */
#define SERVO_RANGE_Y	600		/**< Vertical servo range */

#define SERVO_CENTER	1500	/**< Servo center position */
#define SERVO_ORIGIN_X	(SERVO_CENTER - (SERVO_RANGE_X >> 1))
#define SERVO_ORIGIN_Y	(SERVO_CENTER - (SERVO_RANGE_Y >> 1))

#define SERVO_RATE_X	120		/**< Horizontal move rate (ms/deg) */
#define SERVO_RATE_Y	60		/**< Vertical move rate (ms/deg) */

/** Servo active object structure */
struct servo_ao {
	QActive super;
	vec2f_t cur_pos;
	vec2f_t new_pos;
};

static QState servo_initial(struct servo_ao *me);
static QState servo_idle(struct servo_ao *me);
static QState servo_moving(struct servo_ao *me);

static void servo_set_pos(int servo, float pos);

/** Servo active object */
struct servo_ao servo_ao;

enum timeouts {
	TIMEOUT_MIN_DELAY = TICKS(100),
};


/**
 * Constructor.
 */
void servo_ctor(void)
{
	servo_ao.cur_pos.x = 180.0;
	servo_ao.cur_pos.y = 90.0;

	// Setup PWM, Phase and Frequency correct, prescaler 8, enable OC1A and OC1B
	TCCR1A = (2 << COM1A0) | (2 << COM1B0) | (0 << WGM10);
	TCCR1B = (2 << WGM12) | (2 << CS10);

	// Set TOP to 20000 for 50 Hz PWM
	ICR1 = 20000;

	// Initial servo position
	OCR1A = SERVO_CENTER;
	OCR1B = SERVO_CENTER;

	// Enable OC1A and OC1B pins
	DDRB |= (_BV(5) | _BV(6));

	QActive_ctor((QActive *) &servo_ao, (QStateHandler) servo_initial);
}

/**
 * Moves the servos in a new position.
 * @param v New position in deg
 */
void servo_move(vec2f_t *v)
{
	servo_ao.new_pos = *v;
	QActive_post((QActive *) &servo_ao, SIG_SERVO_MOVE, 0);
}

/**
 * Initial state.
 */
static QState servo_initial(struct servo_ao *me)
{
	return Q_TRAN(servo_idle);
}

/**
 * Idle state.
 */
static QState servo_idle(struct servo_ao *me)
{
	QTimeEvtCtr delay, delay1, delay2;

	switch (Q_SIG(me)) {
	case Q_ENTRY_SIG:
		return Q_HANDLED();
	case Q_EXIT_SIG:
		return Q_HANDLED();
	case Q_TIMEOUT_SIG:
		return Q_HANDLED();
	case SIG_SERVO_MOVE:
		// Compute time needed to move into new position
		DBG("cur_pos %.2f/%.2f\n", me->cur_pos.x, me->cur_pos.y);
		DBG("new_pos %.2f/%.2f\n", me->new_pos.x, me->new_pos.y);
		delay1 = TICKS(fabs(me->new_pos.x - me->cur_pos.x) * SERVO_RATE_X);
		delay2 = TICKS(fabs(me->new_pos.y - me->cur_pos.y) * SERVO_RATE_Y);
		delay = delay1 > delay2 ? delay1 : delay2;
		if (delay < TIMEOUT_MIN_DELAY)
			delay = TIMEOUT_MIN_DELAY;
		DBG("servo delay %d\n", delay * 20);
		servo_set_pos(0, me->new_pos.x / 360.0);
		servo_set_pos(1, me->new_pos.y / 180.0);
		QActive_arm((QActive *) me, delay);
		return Q_TRAN(servo_moving);
	}

	return Q_SUPER(&QHsm_top);
}

/**
 * Moving state.
 */
static QState servo_moving(struct servo_ao *me)
{
	switch (Q_SIG(me)) {
	case Q_ENTRY_SIG:
		return Q_HANDLED();
	case Q_EXIT_SIG:
		return Q_HANDLED();
	case Q_TIMEOUT_SIG:
		me->cur_pos = me->new_pos;
		QActive_post((QActive *) &prog_ao, SIG_SERVO_DONE, 0);
		return Q_TRAN(servo_idle);
	}

	return Q_SUPER(&QHsm_top);
}

/**
 * Sets the servo position.
 * @param servo Servo number
 * @param pos Servo position (0..1)
 */
static void servo_set_pos(int servo, float pos)
{
	unsigned long ocr;

	if (pos < 0.0)
		pos = 0.0;
	if (pos > 1.0)
		pos = 1.0;

	switch (servo) {
	case 0:
		ocr = SERVO_ORIGIN_X + (pos * SERVO_RANGE_X);
		OCR1A = ocr;
		break;
	case 1:
		ocr = SERVO_ORIGIN_Y + (pos * SERVO_RANGE_Y);
		OCR1B = ocr;
		break;
	}
}
