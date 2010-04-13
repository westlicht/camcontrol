/** @file backlight.c
 *
 * Backlight active object. Handles the lcd backlight.
 *
 * Internally uses the TimerXXX TODO to generate variable voltage for LCD
 * backlight.
 *
 * Fades the backlight in, when user presses a key. Fades the backlight out,
 * when no user action took place for a configurable time.
 *
 * @author Simon Kallweit, simon@weirdsoft.ch
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "qpn_port.h"
#include "bsp.h"
#include "camcontrol.h"
#include "defines.h"
#include "param.h"
#include "debug.h"
#include "backlight.h"

/** Backlight active object structure */
struct backlight_ao {
    QActive super;
};

static QState backlight_initial(struct backlight_ao *me);
static QState backlight_off(struct backlight_ao *me);
static QState backlight_on(struct backlight_ao *me);

/** Backlight active object */
struct backlight_ao backlight_ao;

/**
 * Constructor.
 */
void backlight_ctor(void)
{
    QActive_ctor((QActive *) &backlight_ao, (QStateHandler) backlight_initial);
}

/**
 * Initial state.
 */
static QState backlight_initial(struct backlight_ao *me)
{
    return Q_TRAN(backlight_off);
}

/**
 * Switch backlight off.
 */
static QState backlight_off(struct backlight_ao *me)
{
    switch (Q_SIG(me)) {
    case Q_ENTRY_SIG:
        DBG("disable backlight\n"); // TODO
        return Q_HANDLED();
    case Q_EXIT_SIG:
        DBG("enable backlight\n"); // TODO
        return Q_HANDLED();
    case SIG_BACKLIGHT_ACTIVATE:
        return Q_TRAN(backlight_on);
    }

    return Q_SUPER(&QHsm_top);
}

/**
 * Switch backlight on.
 */
static QState backlight_on(struct backlight_ao *me)
{
    switch (Q_SIG(me)) {
    case Q_ENTRY_SIG:
        QActive_arm((QActive *) me, TICKS(pd.misc.backlight * 1000));
        return Q_HANDLED();
    case Q_EXIT_SIG:
        return Q_HANDLED();
    case Q_TIMEOUT_SIG:
        return Q_TRAN(backlight_off);
    case SIG_BACKLIGHT_ACTIVATE:
        QActive_arm((QActive *) me, TICKS(pd.misc.backlight * 1000));
        return Q_HANDLED();
    }

    return Q_SUPER(&QHsm_top);
}
