/*****************************************************************************
* Product: Deferred Event state pattern example
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
#include "defer.h"

#include <stdio.h>           /* this example uses printf() to report status */

/*..........................................................................*/
typedef struct TServerTag {             /* Transaction Server active object */
    QActive super;                                   /* derive from QActive */

    QEvent deferredRequest;                       /* deferred request event */
    /* optionally other storage for deferring other event types... */
} TServer;
                                          /* hierarchical state machine ... */
static QState TServer_initial    (TServer *me);
static QState TServer_operational(TServer *me);
static QState TServer_idle       (TServer *me);
static QState TServer_receiving  (TServer *me);
static QState TServer_authorizing(TServer *me);
static QState TServer_final      (TServer *me);

/* helper functions */
void TServer_deferRequest(TServer *me);
void TServer_recallRequest(TServer *me);

/* Global objects ----------------------------------------------------------*/
TServer AO_TServer;     /* the single instance of the TServer active object */

/*..........................................................................*/
void TServer_ctor(void) {
    QActive_ctor((QActive *)&AO_TServer, (QStateHandler)&TServer_initial);
}
/* HSM definition ----------------------------------------------------------*/
QState TServer_initial(TServer *me) {
    me->deferredRequest.sig = 0;                    /* no deferred requests */
    return Q_TRAN(&TServer_operational);
}
/*..........................................................................*/
QState TServer_final(TServer *me) {
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
QState TServer_operational(TServer *me) {
    switch (Q_SIG(me)) {
        case Q_INIT_SIG: {
            return Q_TRAN(&TServer_idle);
        }
        case NEW_REQUEST_SIG: {
            TServer_deferRequest(me);                  /* defer the request */
            return Q_HANDLED();
        }
        case TERMINATE_SIG: {
            return Q_TRAN(&TServer_final);
        }
    }
    return Q_SUPER(&QHsm_top);
}
/*..........................................................................*/
QState TServer_idle(TServer *me) {
    switch (Q_SIG(me)) {
        case Q_ENTRY_SIG: {
            printf("-> idle\n");
            TServer_recallRequest(me);                /* recall the request */
            return Q_HANDLED();
        }
        case NEW_REQUEST_SIG: {
            printf("Processing request #%d\n", (int)Q_PAR(me));
            return Q_TRAN(&TServer_receiving);
        }
    }
    return Q_SUPER(&TServer_operational);
}
/*..........................................................................*/
QState TServer_receiving(TServer *me) {
    switch (Q_SIG(me)) {
        case Q_ENTRY_SIG: {
            printf("-> receiving\n");
                                            /* one-shot timeout in 1 second */
            QActive_arm((QActive *)me, BSP_TICKS_PER_SEC);
            return Q_HANDLED();
        }
        case Q_TIMEOUT_SIG: {
            return Q_TRAN(&TServer_authorizing);
        }
    }
    return Q_SUPER(&TServer_operational);
}
/*..........................................................................*/
QState TServer_authorizing(TServer *me) {
    switch (Q_SIG(me)) {
        case Q_ENTRY_SIG: {
            printf("-> authorizing\n");
                                           /* one-shot timeout in 2 seconds */
            QActive_arm((QActive *)me, 2*BSP_TICKS_PER_SEC);
            return Q_HANDLED();
        }
        case Q_TIMEOUT_SIG: {
            return Q_TRAN(&TServer_idle);
        }
    }
    return Q_SUPER(&TServer_operational);
}

/* helper functions ........................................................*/
void TServer_deferRequest(TServer *me) {
    if (me->deferredRequest.sig == 0) {    /* the request NOT deferred yet? */
        me->deferredRequest.sig = Q_SIG(me);
        me->deferredRequest.par = Q_PAR(me);
        printf("deferring request #%d\n", (int)me->deferredRequest.par);
    }
    else {
        printf("!!! cannot defer request #%d\n", (int)Q_PAR(me));
    }
}
void TServer_recallRequest(TServer *me) {
    if (me->deferredRequest.sig != 0) {    /* the request already deferred? */
        printf("recalling request #%d\n", (int)me->deferredRequest.par);
        QActive_post((QActive *)me,
                     me->deferredRequest.sig, me->deferredRequest.par);

        me->deferredRequest.sig = 0;          /* request no longer deferred */
    }
    else {
        printf("No requests to recall\n");
    }
}

/* test harness ============================================================*/

/* Local-scope objects -----------------------------------------------------*/
static QEvent l_serverQueue[1];

/* QF_active[] array defines all active object control blocks --------------*/
QActiveCB const Q_ROM Q_ROM_VAR QF_active[] = {
    { (QActive *)0,           (QEvent *)0,   0                    },
    { (QActive *)&AO_TServer, l_serverQueue, Q_DIM(l_serverQueue) }
};

/* make sure that the QF_active[] array matches QF_MAX_ACTIVE in qpn_port.h */
Q_ASSERT_COMPILE(QF_MAX_ACTIVE == Q_DIM(QF_active) - 1);

/*..........................................................................*/
void main (void) {
    TServer_ctor();

    BSP_init();                                     /* initialize the board */

    QF_run();                                /* transfer control to QF-nano */
}
/*..........................................................................*/
void onConsoleInput(uint8_t key) {
    switch (key) {
        case 'n': {                                         /* new request? */
            static uint8_t reqCtr = 0;                /* count the requests */
            QActive_post((QActive *)&AO_TServer, NEW_REQUEST_SIG, ++reqCtr);
            break;
        }
        case 0x1B: {                                        /* ESC pressed? */
            QActive_post((QActive *)&AO_TServer, TERMINATE_SIG, 0);
            break;
        }
    }
}
