/*****************************************************************************
* Product: "Fly 'n' Shoot" game example
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
#include "qpn_port.h"                                       /* QP-nano port */
#include "bsp.h"                             /* Board Support Package (BSP) */
#include "game.h"                                  /* application interface */

/*..........................................................................*/
static QEvent l_tunnelQueue[GAME_MINES_MAX + 4];
static QEvent l_shipQueue[2];
static QEvent l_missileQueue[2];

/* QF_active[] array defines all active object control blocks --------------*/
QActiveCB const Q_ROM Q_ROM_VAR QF_active[] = {
    { (QActive *)0,           (QEvent *)0,    0                     },
    { (QActive *)&AO_Tunnel,  l_tunnelQueue,  Q_DIM(l_tunnelQueue)  },
    { (QActive *)&AO_Ship,    l_shipQueue,    Q_DIM(l_shipQueue)    },
    { (QActive *)&AO_Missile, l_missileQueue, Q_DIM(l_missileQueue) }
};

/* make sure that the QF_active[] array matches QF_MAX_ACTIVE in qpn_port.h */
Q_ASSERT_COMPILE(QF_MAX_ACTIVE == Q_DIM(QF_active) - 1);

/*..........................................................................*/
void main (void) {
    Tunnel_ctor();
    Ship_ctor();
    Missile_ctor(GAME_MISSILE_SPEED_X);

    BSP_init();                                     /* initialize the board */

    QF_run();                                /* transfer control to QF-nano */
}
