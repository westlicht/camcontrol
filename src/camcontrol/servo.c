/**
 * Servo control using 16-bit PWM (timer1).
 */

#include <avr/io.h>
#include "qpn_port.h"
#include "bsp.h"
#include "camcontrol.h"
#include "defines.h"
#include "servo.h"


/** Servo active object structure */
struct servo_ao {
	QActive super;
	uint32_t seconds;
};

static QState servo_initial(struct servo_ao *me);
static QState servo_idle(struct servo_ao *me);

/** Servo active object */
struct servo_ao servo_ao;

enum timeouts {
	TIMEOUT_SECOND = TICKS(1000),
};


/**
 * Constructor.
 */
void servo_ctor(void)
{
	// Setup PWM, Phase and Frequency correct, prescaler 8, enable OC1A and OC1B
	TCCR1A = (2 << COM1A0) | (2 << COM1B0) | (0 << WGM10);
	TCCR1B = (2 << WGM12) | (2 << CS10);

	// Set TOP to 20000 for 50 Hz PWM
	ICR1 = 20000;

	// Initial servo position
	OCR1A = 1500;
	OCR1B = 1500;

	// Enable OC1A and OC1B pins
	DDRB |= (_BV(5) | _BV(6));

	QActive_ctor((QActive *) &servo_ao, (QStateHandler) servo_initial);
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
	switch (Q_SIG(me)) {
	case Q_ENTRY_SIG:
		return Q_HANDLED();
	case Q_EXIT_SIG:
		return Q_HANDLED();
	case Q_TIMEOUT_SIG:
		return Q_HANDLED();
	}

	return Q_SUPER(&QHsm_top);
}

/**
 * Sets the servo position.
 * @param servo Servo number
 * @param pos Servo position (0..1)
 */
void servo_set_pos(int servo, float pos)
{
	unsigned long ocr;

	if (pos < 0.0)
		pos = 0.0;
	if (pos > 1.0)
		pos = 1.0;

	ocr = 1000 + (pos * 1000.0);

	switch (servo) {
	case 0:
		OCR1A = ocr;
		break;
	case 1:
		OCR1B = ocr;
		break;
	}
}
