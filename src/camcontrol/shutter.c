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
    int hdr_shot;
};

static QState shutter_initial(struct shutter_ao *me);
static QState shutter_idle(struct shutter_ao *me);
static QState shutter_long(struct shutter_ao *me);
static QState shutter_hdr(struct shutter_ao *me);
static QState shutter_post_delay(struct shutter_ao *me);

/** Shutter active object */
struct shutter_ao shutter_ao;

enum timeouts {
    TIMEOUT_SECOND = TICKS(1000),
};


#define TIMER_INTERVAL  4       /**< Timer interval in us */

#define SHUTTER_DEBUG

#define SHUTTER_INIT()  DDRB |= _BV(0)

#ifdef SHUTTER_DEBUG
# define SHUTTER_ON()       \
    do {                    \
        PORTB |= _BV(0);    \
        DBG_LED_ON();       \
    } while (0)
# define SHUTTER_OFF()      \
    do {                    \
        PORTB &= ~_BV(0);   \
        DBG_LED_OFF();      \
    } while (0)
#else
# define SHUTTER_ON()   PORTB |= _BV(0)
# define SHUTTER_OFF()  PORTB &= ~_BV(0)
#endif


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
        SHUTTER_ON();
        _delay_ms(ms);
        SHUTTER_OFF();
    } else {
        SHUTTER_ON();
        while (ms >= 1000.0) {
            _delay_ms(1000.0);
            ms -= 1000.0;
        }
        _delay_ms(ms);
        SHUTTER_OFF();
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
        switch (pd.shutter.mode) {
        case SHUTTER_MODE_CAMERA:
            shutter_trigger(200000);
            return Q_TRAN(shutter_post_delay);
        case SHUTTER_MODE_SHORT:
            shutter_trigger(exposure_info[pd.shutter.time].us);
            return Q_TRAN(shutter_post_delay);
        case SHUTTER_MODE_LONG:
            return Q_TRAN(shutter_long);
        case SHUTTER_MODE_HDR:
            return Q_TRAN(shutter_hdr);
        }
        QActive_post((QActive *) &prog_ao, SIG_SHUTTER_DONE, 0);
        return Q_HANDLED();
    case SIG_SHUTTER_STOP:
        return Q_HANDLED();
    }

    return Q_SUPER(&QHsm_top);
}

static QState shutter_long(struct shutter_ao *me)
{
    switch (Q_SIG(me)) {
    case Q_ENTRY_SIG:
        QActive_arm((QActive *) me, TICKS(pd.shutter.long_time * 1000));
        SHUTTER_ON();
        return Q_HANDLED();
    case Q_EXIT_SIG:
        SHUTTER_OFF();
        QActive_disarm((QActive *) me);
        return Q_HANDLED();
    case Q_TIMEOUT_SIG:
        return Q_TRAN(shutter_post_delay);
    case SIG_SHUTTER_STOP:
        return Q_TRAN(shutter_idle);
    }

    return Q_SUPER(&QHsm_top);
}

static QState shutter_hdr(struct shutter_ao *me)
{
    uint32_t min = exposure_info[pd.shutter.hdr_time1].us;
    uint32_t max = exposure_info[pd.shutter.hdr_time2].us;
    uint32_t us;

    switch (Q_SIG(me)) {
    case Q_ENTRY_SIG:
        me->hdr_shot = 0;
        QActive_post((QActive *) me, SIG_SHUTTER_STEP, 0);
        return Q_HANDLED();
    case Q_EXIT_SIG:
        QActive_disarm((QActive *) me);
        return Q_HANDLED();
    case Q_TIMEOUT_SIG:
        QActive_post((QActive *) me, SIG_SHUTTER_STEP, 0);
        return Q_HANDLED();
    case SIG_SHUTTER_STOP:
        return Q_TRAN(shutter_idle);
    case SIG_SHUTTER_STEP:
        us = min + ((max - min) / (pd.shutter.hdr_shots - 1)) * me->hdr_shot;
        shutter_trigger(us);
        me->hdr_shot++;
        if (me->hdr_shot < pd.shutter.hdr_shots) {
            QActive_arm((QActive *) me, TICKS(pd.shutter.post_delay * 10));
            return Q_HANDLED();
        } else {
            return Q_TRAN(shutter_post_delay);
        }
    }

    return Q_SUPER(&QHsm_top);
}

static QState shutter_post_delay(struct shutter_ao *me)
{
    switch (Q_SIG(me)) {
    case Q_ENTRY_SIG:
        QActive_arm((QActive *) me, TICKS(pd.shutter.post_delay * 10));
        return Q_HANDLED();
    case Q_EXIT_SIG:
        QActive_disarm((QActive *) me);
        return Q_HANDLED();
    case Q_TIMEOUT_SIG:
        QActive_post((QActive *) &prog_ao, SIG_SHUTTER_DONE, 0);
        return Q_TRAN(shutter_idle);
    case SIG_SHUTTER_STOP:
        return Q_TRAN(shutter_idle);
    }

    return Q_SUPER(&QHsm_top);
}
