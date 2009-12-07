/*****************************************************************************
* Product: QHsmTst example
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

#include <yfuns.h>               /* for terminal I/O __read() and __write() */
#include <string.h>                                         /* for strlen() */

/*..........................................................................*/
void BSP_init(void) {
    WDTCTL = WDTPW + WDTHOLD;                                   /* Stop WDT */
    P1DIR |= 0x01;                                           /* P1.0 output */
}
/*..........................................................................*/
void BSP_exit(void) {
}
/*..........................................................................*/
void BSP_display(char const *msg) {
    __write(1, (unsigned char const *)msg, strlen(msg));
}
/*..........................................................................*/
void Q_onAssert(char const Q_ROM * const Q_ROM_VAR file, int line) {
    (void)file;                                   /* avoid compiler warning */
    (void)line;                                   /* avoid compiler warning */
    for (;;) {
    }
}
