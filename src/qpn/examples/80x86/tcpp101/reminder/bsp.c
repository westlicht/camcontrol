/*****************************************************************************
* Product: DOS console-based BSP, Turbo C++ 1.01
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

#include <conio.h>
#include <stdlib.h>
#include <stdio.h>

/* Local-scope objects -----------------------------------------------------*/
static void interrupt (*l_dosTmrISR)();

#define TMR_VECTOR      0x08
/*..........................................................................*/
static void interrupt tmrISR() {
    enable();                                          /* unlock interrupts */
    QF_tick();                              /*<-- process the QF clock tick */
    disable();                                     /* lock interrupts again */
    outportb(0x20, 0x20);                    /* write EOI to the master PIC */
}
/*..........................................................................*/
void BSP_init(void) {
}
/*..........................................................................*/
void QF_onStartup(void) {
                                      /* save the origingal DOS vectors ... */
    l_dosTmrISR   = getvect(TMR_VECTOR);

    QF_INT_LOCK();
    setvect(TMR_VECTOR, &tmrISR);
    QF_INT_UNLOCK();
}
/*..........................................................................*/
void QF_stop(void) {
                                    /* restore the original DOS vectors ... */
    if (l_dosTmrISR != (void interrupt (*)(void))0) { /* DOS vectors saved? */
        QF_INT_LOCK();
        setvect(TMR_VECTOR, l_dosTmrISR);
        QF_INT_UNLOCK();
    }
    _exit(0);                                                /* exit to DOS */
}
/*..........................................................................*/
void QF_onIdle(void) {              /* NOTE: entered with interrupts LOCKED */
    enable();                            /* must at least unlock interrupts */

    if (kbhit()) {
        onConsoleInput((uint8_t)getch());
    }
}
/*..........................................................................*/
void Q_onAssert(char const Q_ROM * const Q_ROM_VAR file, int line) {
    fprintf(stderr, "Assertion failed in %s, line %d", file, line);
    QF_stop();
}

/*****************************************************************************
* NOTE01:
* The sofware interrupt generated at the end of the tick ISR invokes the
* original DOS tick ISR handler, which was installed at the spare vector
* 0x81. The DOS tick ISR handler generates the EOI (End-Of-Interrupt)
* to the master 8259A PIC.
*/