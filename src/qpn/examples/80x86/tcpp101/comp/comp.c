/*****************************************************************************
* Product: Orthogonal Component state pattern example
* Last Updated for Version: 4.0.00
* Date of the Last Update:  Mar 11, 2008
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) 2002-2008 Quantum Leaps, LLC. All rights reserved.
*
* This software may be distributed and modified under the terms of the GNU
* General Public License version 2 (GPL) as published by the Free Software
* Foundation and appearing in the file GPL.TXT included in the packaging of
* this file. Please note that GPL Section 2[b] requires that all works based
* on this software must also be made publicly available under the terms of
* the GPL ("Copyleft").
*
* Alternatively, this software may be distributed and modified under the
* terms of Quantum Leaps commercial licenses, which expressly supersede
* the GPL and are specifically designed for licensees interested in
* retaining the proprietary status of their code.
*
* Contact information:
* Quantum Leaps Web site:  http://www.quantum-leaps.com
* e-mail:                  info@quantum-leaps.com
*****************************************************************************/
#include "qpn_port.h"
#include "bsp.h"
#include "comp.h"

#include <stdio.h>

/*..........................................................................*/
typedef struct AlarmClockTag {              /* the AlarmClock active object */
    QActive super;                                   /* derive from QActive */
    Alarm alarmComp;                          /* Alarm orthogonal component */
    uint32_t currentTime;                    /* the current time in seconds */
} AlarmClock;

static QState AlarmClock_initial    (AlarmClock *me);
static QState AlarmClock_timekeeping(AlarmClock *me);
static QState AlarmClock_mode12hr   (AlarmClock *me);
static QState AlarmClock_mode24hr   (AlarmClock *me);
static QState AlarmClock_final      (AlarmClock *me);

/* Global objects ----------------------------------------------------------*/
AlarmClock AO_AlarmClock;       /* the single instance of the AlarmClock AO */

/*..........................................................................*/
void AlarmClock_ctor(void) {
    QActive_ctor((QActive *)&AO_AlarmClock,
                 (QStateHandler)&AlarmClock_initial);
    Alarm_ctor(&AO_AlarmClock.alarmComp);      /* orthogonal component ctor */
}
/* HSM definition ----------------------------------------------------------*/
QState AlarmClock_initial(AlarmClock *me) {
    me->currentTime = 0;
                   /* take the initial transition in the alarm component... */
    QHsm_init((QHsm *)&me->alarmComp);
    return Q_TRAN(&AlarmClock_timekeeping);
}
/*..........................................................................*/
QState AlarmClock_final(AlarmClock *me) {
    switch (Q_SIG(me)) {
        case Q_ENTRY_SIG: {
            printf("-> final\n");
            printf("\nBye!Bye!\n");
            QF_stop();                         /* terminate the application */
            return Q_HANDLED();
        }
    }
    return Q_SUPER(&QHsm_top);
}
/*..........................................................................*/
QState AlarmClock_timekeeping(AlarmClock *me) {
    switch (Q_SIG(me)) {
        case Q_ENTRY_SIG: {
                                                   /* timeout in one second */
            QActive_arm((QActive *)me, BSP_TICKS_PER_SEC);
            return Q_HANDLED();
        }
        case Q_EXIT_SIG: {
            QActive_disarm((QActive *)me);
            return Q_HANDLED();
        }
        case Q_INIT_SIG: {
            return Q_TRAN(&AlarmClock_mode24hr);
        }
        case CLOCK_12H_SIG: {
            return Q_TRAN(&AlarmClock_mode12hr);
        }
        case CLOCK_24H_SIG: {
            return Q_TRAN(&AlarmClock_mode24hr);
        }
        case ALARM_SIG: {
            printf("Wake up!!!\n");
            return Q_HANDLED();
        }
        case ALARM_SET_SIG:
        case ALARM_ON_SIG:
        case ALARM_OFF_SIG: {
                      /* synchronously dispatch to the orthogonal component */
            Q_SIG(&me->alarmComp) = Q_SIG(me);
            Q_PAR(&me->alarmComp) = Q_PAR(me);
            QHsm_dispatch((QHsm *)&me->alarmComp);
            return Q_HANDLED();
        }
        case TERMINATE_SIG: {
            return Q_TRAN(&AlarmClock_final);
        }
    }
    return Q_SUPER(&QHsm_top);
}
/*..........................................................................*/
QState AlarmClock_mode24hr(AlarmClock *me) {
    switch (Q_SIG(me)) {
        case Q_ENTRY_SIG: {
            printf("*** 24-hour mode\n");
            return Q_HANDLED();
        }
        case Q_TIMEOUT_SIG: {
                                                   /* timeout in one second */
            QActive_arm((QActive *)me, BSP_TICKS_PER_SEC);

            if (++me->currentTime == 24*60) {   /* roll over in 24-hr mode? */
                me->currentTime = 0;
            }
            printf("%02ld:%02ld\n",
                   me->currentTime/60, me->currentTime%60);

                      /* synchronously dispatch to the orthogonal component */
            Q_SIG(&me->alarmComp) = TIME_SIG;
            Q_PAR(&me->alarmComp) = me->currentTime;
            QHsm_dispatch((QHsm *)&me->alarmComp);
            return Q_HANDLED();
        }
    }
    return Q_SUPER(&AlarmClock_timekeeping);
}
/*..........................................................................*/
QState AlarmClock_mode12hr(AlarmClock *me) {
    switch (Q_SIG(me)) {
        case Q_ENTRY_SIG: {
            printf("*** 12-hour mode\n");
            return Q_HANDLED();
        }
        case Q_TIMEOUT_SIG: {
            uint32_t h;                  /* temporary variable to hold hour */
                                                   /* timeout in one second */
            QActive_arm((QActive *)me, BSP_TICKS_PER_SEC);

            if (++me->currentTime == 12*60) {   /* roll over in 12-hr mode? */
                me->currentTime = 0;
            }
            h = me->currentTime/60;
            printf("%02ld:%02ld %s\n", (h % 12) ? (h % 12) : 12,
                   me->currentTime % 60, (h / 12) ? "PM" : "AM");

                      /* synchronously dispatch to the orthogonal component */
            Q_SIG(&me->alarmComp) = TIME_SIG;
            Q_PAR(&me->alarmComp) = me->currentTime;
            QHsm_dispatch((QHsm *)&me->alarmComp);
            return Q_HANDLED();
        }
    }
    return Q_SUPER(&AlarmClock_timekeeping);
}

/* test harness ============================================================*/

/* Local-scope objects -----------------------------------------------------*/
static QEvent l_clockQueue[2];

/* QF_active[] array defines all active object control blocks --------------*/
QActiveCB const Q_ROM Q_ROM_VAR QF_active[] = {
    { (QActive *)0,              (QEvent *)0,   0                   },
    { (QActive *)&AO_AlarmClock, l_clockQueue, Q_DIM(l_clockQueue) }
};
uint8_t const Q_ROM QF_activeNum = Q_DIM(QF_active) - 1;

/*..........................................................................*/
void main (void) {
    AlarmClock_ctor();

    printf("Orthogonal Component state pattern\nQP-nano version: %s\n"
           "Press 'o' to turn the Alarm ON\n"
           "Press 'f' to turn the Alarm OFF\n"
           "Press '0'..'9' to set the Alarm time\n"
           "Press 'A' to set the Clock in 12-hour mode\n"
           "Press 'B' to set the Clock in 24-hour mode\n"
           "Press ESC to quit...\n",
           QP_getVersion());


    BSP_init();                                     /* initialize the board */

    QF_run();                                /* transfer control to QF-nano */
}
/*..........................................................................*/
void onConsoleInput(uint8_t key) {
    switch (key) {
        case 'o': {                                    /* Alarm 'o'n event? */
            QActive_post((QActive *)&AO_AlarmClock, ALARM_ON_SIG, 0);
            break;
        }
        case 'f': {                                   /* Alarm o'f'f event? */
            QActive_post((QActive *)&AO_AlarmClock, ALARM_OFF_SIG, 0);
            break;
        }
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9': {                                     /* Alarm set event? */
            QActive_post((QActive *)&AO_AlarmClock, ALARM_SET_SIG, key - '0');
            break;
        }
        case 'A': {                                     /* Clock 12H event? */
            QActive_post((QActive *)&AO_AlarmClock, CLOCK_12H_SIG, 0);
            break;
        }
        case 'B': {                                     /* Clock 24H event? */
            QActive_post((QActive *)&AO_AlarmClock, CLOCK_24H_SIG, 0);
            break;
        }
        case 0x1B: {                                        /* ESC pressed? */
            QActive_post((QActive *)&AO_AlarmClock, TERMINATE_SIG, 0);
            break;
        }
    }
}
