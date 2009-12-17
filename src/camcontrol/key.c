
#include <stdint.h>
#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "qpn_port.h"
#include "camcontrol.h"
#include "key.h"
#include "debug.h"

#define NUM_KEYS		5
#define NUM_DEBOUNCE	10
#define KEY_PORT		A

#define GLUE(a, b)		a##b
#define PIN(x)			GLUE(PIN, x)
#define KEY_INPORT		PIN(KEY_PORT)

static uint8_t debounce[NUM_KEYS];

static void press(int index);
static void release(int index);

ISR(TIMER2_COMP_vect)
{
	int i;

	for (i = 0; i < NUM_KEYS; i++) {
		if (KEY_INPORT & _BV(i)) {
			// Key is up
			if (debounce[i] != 0) {
				release(i);
				debounce[i] = 0;
			}
		} else {
			// Key is down
			if (debounce[i] < NUM_DEBOUNCE) {
				debounce[i]++;
			} else {
				if (debounce[i] == NUM_DEBOUNCE) {
					press(i);
					debounce[i]++;
				}
			}
		}
	}
}

void key_init(void)
{
    // Set compare register
    OCR2 = 32;
	// Set Timer2 in CTC mode, 1/1024 prescaler
    TCCR2 = ((1 << WGM01) | (0 << WGM00) | (5 << CS00));
    // Enable compare interrupt
    TIMSK |= _BV(OCIE2);
}


static void press(int index)
{
	QActive_postISR((QActive *) &mmi_ao, SIG_KEY_PRESS, index);
}

static void release(int index)
{
	QActive_postISR((QActive *) &mmi_ao, SIG_KEY_RELEASE, index);
}
