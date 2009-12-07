/*****************************************************************************
* Product: DPP example
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
#include "dpp.h"                                   /* application interface */
#include "bsp.h"                             /* Board Support Package (BSP) */

/*..........................................................................*/
static QEvent l_philoQueue[N_PHILO][N_PHILO];
static QEvent l_tableQueue[2];

/* QF_active[] array defines all active object control blocks --------------*/
QActiveCB const Q_ROM Q_ROM_VAR QF_active[] = {
    { (QActive *)0,           (QEvent *)0,      0                       },
    { (QActive *)&AO_Philo0,  l_philoQueue[0],  Q_DIM(l_philoQueue[0])  },
    { (QActive *)&AO_Philo1,  l_philoQueue[1],  Q_DIM(l_philoQueue[1])  },
    { (QActive *)&AO_Philo2,  l_philoQueue[2],  Q_DIM(l_philoQueue[2])  },
    { (QActive *)&AO_Philo3,  l_philoQueue[3],  Q_DIM(l_philoQueue[3])  },
    { (QActive *)&AO_Philo4,  l_philoQueue[4],  Q_DIM(l_philoQueue[4])  },
    { (QActive *)&AO_Table,   l_tableQueue,     Q_DIM(l_tableQueue)     }
};

/* make sure that the QF_active[] array matches QF_MAX_ACTIVE in qpn_port.h */
Q_ASSERT_COMPILE(QF_MAX_ACTIVE == Q_DIM(QF_active) - 1);

/*..........................................................................*/
int main(int argc, char *argv[]) {
    Philo_ctor();                              /* instantiate all Philo AOs */
    Table_ctor();                               /* instantiate the Table AO */

    BSP_init(argc, argv);                           /* initialize the board */

    QF_run();                                /* transfer control to QF-nano */

    return 0;
}
