/** @file servo.c
 *
 * Servo active object. Handles movement of the servos.
 *
 * Internally uses the Timer1 to generate PWM signals for the two servos.
 *
 * @author Simon Kallweit, simon@weirdsoft.ch
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <math.h>
#include "qpn_port.h"
#include "bsp.h"
#include "camcontrol.h"
#include "defines.h"
#include "prog.h"
#include "param.h"
#include "debug.h"
#include "utils.h"
#include "servo.h"

#define SERVO_CENTER    1500    /**< Servo center position */
#define SERVO_RANGE     800     /**< Total servo range */
#define SERVO_ORIGIN    (SERVO_CENTER - SERVO_RANGE)

#define SERVO_VEL       30      /**< Servo velocity (0.1 steps per 20ms) */

/** Servo active object structure */
struct servo_ao {
    QActive super;
    vec2f_t goal;
    vec2i_t pos;
    vec2i_t ofs;
};

static QState servo_initial(struct servo_ao *me);
static QState servo_center(struct servo_ao *me);
static QState servo_idle(struct servo_ao *me);
static QState servo_moving(struct servo_ao *me);

static void compute_pos(vec2f_t *deg, vec2i_t *pos);

/** Servo active object */
struct servo_ao servo_ao;

enum timeouts {
    TIMEOUT_WAIT_CENTER = TICKS(3000),
    TIMEOUT_POST_DELAY = TICKS(3000),
};


ISR(TIMER1_OVF_vect)
{
    struct servo_ao *me = &servo_ao;
    int16_t vel;

    // Move X servo
    if (me->ofs.x) {
        vel = CLAMP(me->ofs.x, -SERVO_VEL, SERVO_VEL);
        me->ofs.x -= vel;
        me->pos.x += vel;
        OCR1A = (me->pos.x / 10);
        if ((me->ofs.x == 0) && (me->ofs.y == 0))
            QActive_postISR((QActive *) me, SIG_SERVO_DONE, 0);
    }

    // Move Y servo
    if (me->ofs.y) {
        vel = CLAMP(me->ofs.y, -SERVO_VEL, SERVO_VEL);
        me->ofs.y -= vel;
        me->pos.y += vel;
        OCR1B = (me->pos.y / 10);
        if ((me->ofs.x == 0) && (me->ofs.y == 0))
            QActive_postISR((QActive *) me, SIG_SERVO_DONE, 0);
    }
}

/**
 * Constructor.
 */
void servo_ctor(void)
{
    vec2(&servo_ao.goal, deg2rad(180.0), deg2rad(90.0));
    compute_pos(&servo_ao.goal, &servo_ao.pos);
    vec2(&servo_ao.ofs, 0, 0);

    // Setup PWM, Phase and Frequency correct, prescaler 8, enable OC1A and OC1B
    TCCR1A = (2 << COM1A0) | (2 << COM1B0) | (0 << WGM10);
    TCCR1B = (2 << WGM12) | (2 << CS10);

    // Set TOP to 20000 for 50 Hz PWM
    ICR1 = 20000;

    // Initial servo position
    OCR1A = servo_ao.pos.x / 10;
    OCR1B = servo_ao.pos.y / 10;

    TIMSK |= _BV(TOV1);

    // Enable OC1A and OC1B pins
    DDRB |= (_BV(5) | _BV(6));

    QActive_ctor((QActive *) &servo_ao, (QStateHandler) servo_initial);
}

/**
 * Moves the servos in a new position.
 * @param v New position in radians
 */
void servo_move(vec2f_t *v)
{
    struct servo_ao *me = &servo_ao;
    vec2i_t new_pos;

    servo_ao.goal = *v;

    TIMSK &= ~_BV(TOV1);
    compute_pos(&servo_ao.goal, &new_pos);
    me->ofs.x = new_pos.x - me->pos.x;
    me->ofs.y = new_pos.y - me->pos.y;
    TIMSK |= _BV(TOV1);

    QActive_post((QActive *) &servo_ao, SIG_SERVO_MOVE, 0);
}

/**
 * Returns the current goal position of the servos.
 * @param v Goal position in radians
 */
void servo_get_goal(vec2f_t *v)
{
    *v = servo_ao.goal;
}

/**
 * Initial state.
 */
static QState servo_initial(struct servo_ao *me)
{
    return Q_TRAN(servo_center);
}

/**
 * Wait until the servos are centered.
 */
static QState servo_center(struct servo_ao *me)
{
    switch (Q_SIG(me)) {
    case Q_ENTRY_SIG:
        QActive_arm((QActive *) me, TIMEOUT_WAIT_CENTER);
        return Q_HANDLED();
    case Q_EXIT_SIG:
        return Q_HANDLED();
    case Q_TIMEOUT_SIG:
        return Q_TRAN(servo_idle);
    }

    return Q_SUPER(&QHsm_top);
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
    case SIG_SERVO_MOVE:
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
        QActive_post((QActive *) &prog_ao, SIG_SERVO_DONE, 0);
        return Q_TRAN(servo_idle);
    case SIG_SERVO_MOVE:
        QActive_disarm((QActive *) me);
        return Q_HANDLED();
    case SIG_SERVO_DONE:
        QActive_arm((QActive *) me, TIMEOUT_POST_DELAY);
        return Q_HANDLED();
    }

    return Q_SUPER(&QHsm_top);
}


static void compute_pos(vec2f_t *deg, vec2i_t *pos)
{
    vec2f_t v;
    v.x = deg->x / M_2PI;
    v.y = deg->y / M_PI;
    v.x = CLAMP(v.x, 0.0, 1.0);
    v.y = CLAMP(v.y, 0.0, 1.0);

    pos->x = (SERVO_ORIGIN + pd.servo.min_x +
              (v.x * (pd.servo.max_x - pd.servo.min_x))) * 10;
    pos->y = (SERVO_ORIGIN + pd.servo.min_y +
              (v.y * (pd.servo.max_y - pd.servo.min_y))) * 10;
}
