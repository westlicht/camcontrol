/** @file key.c
 *
 * Keyboard and rotary encoder handling.
 *
 * Uses Timer0 hardware timer (shared with system tick) to periodically
 * scan the keys and encoder. Does debouncing and posts events to the mmi
 * active object for key presses, releases and encoder turning.
 *
 * @author Simon Kallweit, simon@weirdsoft.ch
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "qpn_port.h"
#include "camcontrol.h"
#include "debug.h"
#include "mmi.h"
#include "backlight.h"
#include "key.h"

#define NUM_KEYS        5
#define NUM_DEBOUNCE    30
#define KEY_PORT        A

#define ENC_PORT        B
#define ENC_PIN_UP      2
#define ENC_PIN_DOWN    3

#define GLUE(a, b)      a##b
#define PIN(x)          GLUE(PIN, x)

#define PIN_KEY         PIN(KEY_PORT)
#define PIN_ENC         PIN(ENC_PORT)

static uint8_t debounce[NUM_KEYS];
static int enc_dir;

static void press(int index);
static void release(int index);
static void encoder(int dir);

/**
 * Initialies the keyboard driver. Sets ports.
 */
void key_init(void)
{
    // Set button pins
    PORTA &= ~0x1f;
    DDRA &= ~0x1f;

    // Set ENC pins
    PORTB &= ~0x0c;
    DDRB &= ~0x0c;
}

/**
 * Scans the keys and the encoder. This is periodically called from the BSP.
 */
void key_scan(void)
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
        if (!(enc & _BV(ENC_PIN_UP)))
            enc_dir = 1;
        else if (!(enc & _BV(ENC_PIN_DOWN)))
            enc_dir = -1;
        break;
    case 1:
        if (!(enc & _BV(ENC_PIN_DOWN))) {
            encoder(ENC_DOWN);
            enc_dir = 2;
        }
        if (enc & _BV(ENC_PIN_UP))
            enc_dir = 0;
        break;
    case -1:
        if (!(enc & _BV(ENC_PIN_UP))) {
            encoder(ENC_UP);
            enc_dir = 2;
        }
        if (enc & _BV(ENC_PIN_DOWN))
            enc_dir = 0;
        break;
    case 2:
        if ((enc & _BV(ENC_PIN_UP)) && (enc & _BV(ENC_PIN_DOWN)))
            enc_dir = 0;
        break;
    }
}

/**
 * Sends a SIG_KEY_PRESS event to the mmi active object.
 * @param index Key index
 */
static void press(int index)
{
    QActive_postISR((QActive *) &mmi_ao, SIG_KEY_PRESS, index);
    QActive_postISR((QActive *) &backlight_ao, SIG_BACKLIGHT_ACTIVATE, 0);
}

/**
 * Sends a SIG_KEY_RELEASE event to the mmi active object.
 * @param index Key index
 */
static void release(int index)
{
    QActive_postISR((QActive *) &mmi_ao, SIG_KEY_RELEASE, index);
    QActive_postISR((QActive *) &backlight_ao, SIG_BACKLIGHT_ACTIVATE, 0);
}

/**
 * Seds a SIG_ENCODER event to the mmi active object.
 * @param dir Direction
 */
static void encoder(int dir)
{
    QActive_postISR((QActive *) &mmi_ao, SIG_ENCODER, dir);
    QActive_postISR((QActive *) &backlight_ao, SIG_BACKLIGHT_ACTIVATE, 0);
}
