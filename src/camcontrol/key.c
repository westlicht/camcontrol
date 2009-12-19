
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

#define ENC_PORT		B
#define ENC_UP			2
#define ENC_DOWN		3

#define GLUE(a, b)		a##b
#define PIN(x)			GLUE(PIN, x)

#define PIN_KEY			PIN(KEY_PORT)
#define PIN_ENC			PIN(ENC_PORT)

static uint8_t debounce[NUM_KEYS];
static int enc_dir;

static void press(int index);
static void release(int index);
static void encoder(int dir);

ISR(TIMER2_COMP_vect)
{
	int i;
	uint8_t enc;

	// Handle buttons
	for (i = 0; i < NUM_KEYS; i++) {
		if (PIN_KEY & _BV(i)) {
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

	// Handle encodersb
	enc = PIN_ENC;
	switch (enc_dir) {
	case 0:
		if (!(enc & _BV(ENC_UP)))
			enc_dir = 1;
		else if (!(enc & _BV(ENC_DOWN)))
			enc_dir = -1;
		break;
	case 1:
		if (!(enc & _BV(ENC_DOWN))) {
			encoder(1);
			enc_dir = 2;
		}
		if (enc & _BV(ENC_UP))
			enc_dir = 0;
		break;
	case -1:
		if (!(enc & _BV(ENC_UP))) {
			encoder(-1);
			enc_dir = 2;
		}
		if (enc & _BV(ENC_DOWN))
			enc_dir = 0;
		break;
	case 2:
		if ((enc & _BV(ENC_UP)) && (enc & _BV(ENC_DOWN)))
			enc_dir = 0;
		break;
	}
}

void key_init(void)
{
	// Set button pins
	PORTA &= ~0x1f;
	DDRA &= ~0x1f;

	// Set ENC pins
	PORTB &= ~0x0c;
	DDRB &= ~0x0c;

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

static void encoder(int dir)
{
	QActive_postISR((QActive *) &mmi_ao, SIG_ENCODER, dir);
}
