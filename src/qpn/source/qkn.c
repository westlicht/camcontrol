/*****************************************************************************
* Product: QK-nano implemenation
* Last Updated for Version: 4.0.03
* Date of the Last Update:  Jan 01, 2009
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) 2002-2009 Quantum Leaps, LLC. All rights reserved.
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
#include "qpn_port.h"                                       /* QP-nano port */

Q_DEFINE_THIS_MODULE(qkn)

/**
* \file
* \ingroup qkn
* QK-nano implementation.
*/

#ifndef QK_PREEMPTIVE
    #error "The required header file qkn.h is not included in qpn_port.h"
#endif

/* Global-scope objects ----------------------------------------------------*/
                                      /* start with the QK scheduler locked */
uint8_t volatile QK_currPrio_ = (uint8_t)(QF_MAX_ACTIVE + 1);

#ifdef QF_ISR_NEST
uint8_t volatile QK_intNest_;              /* start with nesting level of 0 */
#endif

#ifdef QK_MUTEX
uint8_t volatile QK_ceilingPrio_;            /* ceiling priority of a mutex */
#endif

/* local objects -----------------------------------------------------------*/
static QActive *l_act;                                         /* ptr to AO */

/*..........................................................................*/
void QF_run(void) {
    static uint8_t p;                /* declared static to save stack space */

                         /* set priorities all registered active objects... */
    for (p = (uint8_t)1; p <= (uint8_t)QF_MAX_ACTIVE; ++p) {
        l_act = (QActive *)Q_ROM_PTR(QF_active[p].act);
        Q_ASSERT(l_act != (QActive *)0);/* QF_active[p] must be initialized */
        l_act->prio = p;           /* set the priority of the active object */
    }
         /* trigger initial transitions in all registered active objects... */
    for (p = (uint8_t)1; p <= (uint8_t)QF_MAX_ACTIVE; ++p) {
        l_act = (QActive *)Q_ROM_PTR(QF_active[p].act);
#ifndef QF_FSM_ACTIVE
        QHsm_init((QHsm *)l_act);                          /* initial tran. */
#else
        QFsm_init((QFsm *)l_act);                          /* initial tran. */
#endif
    }

    QF_INT_LOCK();
    QK_currPrio_ = (uint8_t)0;     /* set the priority for the QK idle loop */
    QK_SCHEDULE_();                   /* process all events produced so far */
    QF_INT_UNLOCK();

    QF_onStartup();                             /* invoke startup callback  */

    for (;;) {                                    /* enter the QK idle loop */
        QK_onIdle();                         /* invoke the on-idle callback */
    }
}
/*..........................................................................*/
/* NOTE: the QK scheduler is entered and exited with interrupts LOCKED */
void QK_schedule_(void) Q_REENTRANT {
    static uint8_t const Q_ROM Q_ROM_VAR log2Lkup[] = {
        0, 1, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4
    };
    static uint8_t const Q_ROM Q_ROM_VAR invPow2Lkup[] = {
        0xFF, 0xFE, 0xFD, 0xFB, 0xF7, 0xEF, 0xDF, 0xBF, 0x7F
    };

    uint8_t pin;                            /* the initial QK-nano priority */
    uint8_t p;           /* new highest-priority active object ready to run */

          /* determine the priority of the highest-priority AO ready to run */
#if (QF_MAX_ACTIVE > 4)
    if ((QF_readySet_ & 0xF0) != 0) {                 /* upper nibble used? */
        p = (uint8_t)(Q_ROM_BYTE(log2Lkup[QF_readySet_ >> 4]) + 4);
    }
    else                            /* upper nibble of QF_readySet_ is zero */
#endif
    {
        p = Q_ROM_BYTE(log2Lkup[QF_readySet_]);
    }

#ifdef QK_MUTEX                     /* QK priority-ceiling mutexes allowed? */
    if ((p > QK_currPrio_) && (p > QK_ceilingPrio_)) {
#else
    if (p > QK_currPrio_) {                     /* do we have a preemption? */
#endif                                                          /* QK_MUTEX */

        pin = QK_currPrio_;                    /* save the initial priority */
        do {
            QK_currPrio_ = p;  /* new priority becomes the current priority */
            QF_INT_UNLOCK();          /* unlock interrupts to launch a task */

                                      /* dispatch to HSM (execute RTC step) */
#ifndef QF_FSM_ACTIVE
            QHsm_dispatch((QHsm *)Q_ROM_PTR(QF_active[p].act));
#else
            QFsm_dispatch((QFsm *)Q_ROM_PTR(QF_active[p].act));
#endif

            QF_INT_LOCK();
               /* set cb and a again, in case they change over the RTC step */
            l_act = (QActive *)Q_ROM_PTR(QF_active[p].act);

            if ((--l_act->nUsed) == (uint8_t)0) {/*is queue becoming empty? */
                                                     /* clear the ready bit */
                QF_readySet_ &= Q_ROM_BYTE(invPow2Lkup[p]);
            }
            else {
                Q_SIG(l_act) = ((QEvent *)Q_ROM_PTR(QF_active[p].queue))
                                   [l_act->tail].sig;
#if (Q_PARAM_SIZE != 0)
                Q_PAR(l_act) = ((QEvent *)Q_ROM_PTR(QF_active[p].queue))
                                   [l_act->tail].par;
#endif
                if (l_act->tail == (uint8_t)0) {            /* wrap around? */
                    l_act->tail = Q_ROM_BYTE(QF_active[p].end);/* wrap tail */
                }
                --l_act->tail;
            }
                          /* determine the highest-priority AO ready to run */
            if (QF_readySet_ != (uint8_t)0) {
#if (QF_MAX_ACTIVE > 4)
                if ((QF_readySet_ & 0xF0) != 0) {     /* upper nibble used? */
                    p = (uint8_t)(Q_ROM_BYTE(log2Lkup[QF_readySet_ >> 4])+ 4);
                }
                else                /* upper nibble of QF_readySet_ is zero */
#endif
                {
                    p = Q_ROM_BYTE(log2Lkup[QF_readySet_]);
                }
            }
            else {
                p = (uint8_t)0;                    /* break out of the loop */
            }

#ifdef QK_MUTEX                     /* QK priority-ceiling mutexes allowed? */
        } while ((p > pin) && (p > QK_ceilingPrio_));
#else
        } while (p > pin);
#endif                                                          /* QK_MUTEX */
        QK_currPrio_ = pin;                 /* restore the initial priority */
    }
}

#ifdef QK_MUTEX
/*..........................................................................*/
QMutex QK_mutexLock(uint8_t prioCeiling) {
    uint8_t mutex;
    QF_INT_LOCK();
    mutex = QK_ceilingPrio_;  /* the original QK priority ceiling to return */
    if (QK_ceilingPrio_ < prioCeiling) {
        QK_ceilingPrio_ = prioCeiling;     /* raise the QK priority ceiling */
    }
    QF_INT_UNLOCK();
    return mutex;
}
/*..........................................................................*/
void QK_mutexUnlock(QMutex mutex) {
    QF_INT_LOCK();
    if (QK_ceilingPrio_ > mutex) {
        QK_ceilingPrio_ = mutex;      /* restore the saved priority ceiling */
        QK_SCHEDULE_();
    }
    QF_INT_UNLOCK();
}
#endif                                                   /* #ifdef QK_MUTEX */
