/*****************************************************************************
* Product: Orthogonal Component state pattern example
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
#include "comp.h"

#include <stdio.h>

/* HSM methods */
static QState  Alarm_initial(Alarm *me);
static QState Alarm_alarm  (Alarm *me);
static QState Alarm_on     (Alarm *me);
static QState Alarm_off    (Alarm *me);

/*..........................................................................*/
void Alarm_ctor(Alarm *me) {
    QHsm_ctor(&me->super, (QStateHandler)&Alarm_initial);
}

/* HSM definition ----------------------------------------------------------*/
QState Alarm_initial(Alarm *me) {
    return Q_TRAN(&Alarm_alarm);
}
/*..........................................................................*/
QState Alarm_alarm(Alarm *me) {
    switch (Q_SIG(me)) {
        case Q_INIT_SIG: {
            return Q_TRAN(&Alarm_on);
        }
        case ALARM_ON_SIG: {
            return Q_TRAN(&Alarm_on);
        }
        case ALARM_OFF_SIG: {
            return Q_TRAN(&Alarm_off);
        }
    }
    return Q_SUPER(&QHsm_top);
}
/*..........................................................................*/
QState Alarm_on(Alarm *me) {
    switch (Q_SIG(me)) {
        case Q_ENTRY_SIG: {
            printf("*** Alarm ON %02ld:%02ld\n",
                   me->alarmTime/60, me->alarmTime%60);
            return Q_HANDLED();
        }
        case ALARM_SET_SIG: {
            printf("*** Cannot set Alarm when it is ON\n");
            return Q_HANDLED();
        }
        case TIME_SIG: {
            if (Q_PAR(me) == me->alarmTime) {
                printf("ALARM!!!\n");
                       /* asynchronously post the event to the container AO */
                QActive_post((QActive *)&AO_AlarmClock, ALARM_SIG, 0);
            }
            return Q_HANDLED();
        }
    }
    return Q_SUPER(&Alarm_alarm);
}
/*..........................................................................*/
QState Alarm_off(Alarm *me) {
    switch (Q_SIG(me)) {
        case Q_ENTRY_SIG: {
             /* while in the off state, the alarm is kept in decimal format */
            me->alarmTime = (me->alarmTime/60)*100 + me->alarmTime%60;
            printf("*** Alarm OFF %02ld:%02ld\n",
                   me->alarmTime/100, me->alarmTime%100);
            return Q_HANDLED();
        }
        case Q_EXIT_SIG: {
                      /* upon exit, the alarm is converted to binary format */
            me->alarmTime = (me->alarmTime/100)*60 + me->alarmTime%100;
            return Q_HANDLED();
        }
        case ALARM_SET_SIG: {
                      /* while setting, the alarm is kept in decimal format */
            uint32_t alarm = (10 * me->alarmTime
                              + Q_PAR(me)) % 10000;
            if ((alarm / 100 < 24) && (alarm % 100 < 60)) {/*alarm in range?*/
                me->alarmTime = alarm;
            }
            else {                      /* alarm out of range -- start over */
                me->alarmTime = 0;
            }
            printf("*** Alarm SET %02ld:%02ld\n",
                   me->alarmTime/100, me->alarmTime%100);
            return Q_HANDLED();
        }
    }
    return Q_SUPER(&Alarm_alarm);
}
