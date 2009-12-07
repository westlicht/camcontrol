/*****************************************************************************
* Product: Reminder state pattern example
* Last Updated for Version: 4.0.00
* Date of the Last Update:  Apr 05, 2008
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
#include "reminder.h"

#include <stdio.h>

/*..........................................................................*/
typedef struct SensorTag {
    QActive super;                                   /* derive from QActive */

    uint16_t pollCtr;
    uint16_t procCtr;
} Sensor;
                                          /* hierarchical state machine ... */
static QState Sensor_initial   (Sensor *me);
static QState Sensor_polling   (Sensor *me);
static QState Sensor_processing(Sensor *me);
static QState Sensor_idle      (Sensor *me);
static QState Sensor_busy      (Sensor *me);
static QState Sensor_final     (Sensor *me);

/* Global objects ----------------------------------------------------------*/
Sensor AO_Sensor;

/*..........................................................................*/
void Sensor_ctor(void) {
    QActive_ctor((QActive *)&AO_Sensor, (QStateHandler)&Sensor_initial);
}

/* HSM definition ----------------------------------------------------------*/
QState Sensor_initial(Sensor *me) {
    me->pollCtr = 0;
    me->procCtr = 0;
    return Q_TRAN(&Sensor_polling);
}
/*..........................................................................*/
QState Sensor_final(Sensor *me) {
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
QState Sensor_polling(Sensor *me) {
    switch (Q_SIG(me)) {
        case Q_ENTRY_SIG: {
                                                   /* timeout in 1/2 second */
            QActive_arm((QActive *)me, BSP_TICKS_PER_SEC/2);
            return Q_HANDLED();
        }
        case Q_EXIT_SIG: {
            QActive_disarm((QActive *)me);
            return Q_HANDLED();
        }
        case Q_INIT_SIG: {
            return Q_TRAN(&Sensor_processing);
        }
        case Q_TIMEOUT_SIG: {
                                                   /* timeout in 1/2 second */
            QActive_arm((QActive *)me, BSP_TICKS_PER_SEC/2);
            ++me->pollCtr;
            printf("poll %3d\n", me->pollCtr);
            if ((me->pollCtr & 0x3) == 0) {                     /* modulo 4 */
                QActive_post((QActive *)me, DATA_READY_SIG, 0);
            }
            return Q_HANDLED();
        }
        case TERMINATE_SIG: {
            return Q_TRAN(&Sensor_final);
        }
    }
    return Q_SUPER(&QHsm_top);
}
/*..........................................................................*/
QState Sensor_processing(Sensor *me) {
    switch (Q_SIG(me)) {
        case Q_INIT_SIG: {
            return Q_TRAN(&Sensor_idle);
        }
    }
    return Q_SUPER(&Sensor_polling);
}
/*..........................................................................*/
QState Sensor_idle(Sensor *me) {
    switch (Q_SIG(me)) {
        case Q_ENTRY_SIG: {
            printf("-> idle\n");
            return Q_HANDLED();
        }
        case DATA_READY_SIG: {
            return Q_TRAN(&Sensor_busy);
        }
    }
    return Q_SUPER(&Sensor_processing);
}
/*..........................................................................*/
QState Sensor_busy(Sensor *me) {
    switch (Q_SIG(me)) {
        case Q_ENTRY_SIG: {
            printf("-> busy\n");
            return Q_HANDLED();
        }
        case Q_TIMEOUT_SIG: {
                                                   /* timeout in 1/2 second */
            QActive_arm((QActive *)me, BSP_TICKS_PER_SEC/2);

            ++me->procCtr;
            printf("process %3d\n", me->procCtr);
            if ((me->procCtr & 0x1) == 0) {                     /* modulo 2 */
                return Q_TRAN(&Sensor_idle);
            }
            return Q_HANDLED();
        }
    }
    return Q_SUPER(&Sensor_processing);
}

/* test harness ============================================================*/

/* Local-scope objects -----------------------------------------------------*/
static QEvent l_sensorQueue[1];

/* QF_active[] array defines all active object control blocks --------------*/
QActiveCB const Q_ROM Q_ROM_VAR QF_active[] = {
    { (QActive *)0,          (QEvent *)0,   0                    },
    { (QActive *)&AO_Sensor, l_sensorQueue, Q_DIM(l_sensorQueue) }
};

/* make sure that the QF_active[] array matches QF_MAX_ACTIVE in qpn_port.h */
Q_ASSERT_COMPILE(QF_MAX_ACTIVE == Q_DIM(QF_active) - 1);

/*..........................................................................*/
void main (void) {
    Sensor_ctor();

    BSP_init();                                     /* initialize the board */

    QF_run();                                /* transfer control to QF-nano */
}
/*..........................................................................*/
void onConsoleInput(uint8_t key) {
    switch (key) {
        case 0x1B: {                                        /* ESC pressed? */
            QActive_post((QActive *)&AO_Sensor, TERMINATE_SIG, 0);
            break;
        }
    }
}
