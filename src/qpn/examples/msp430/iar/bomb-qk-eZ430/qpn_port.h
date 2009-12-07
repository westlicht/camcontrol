/*****************************************************************************
* Product: Time Bomb example (FSM) with QK-nano
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
#ifndef qpn_port_h
#define qpn_port_h

#define Q_NHSM
#define Q_PARAM_SIZE            1
#define QF_TIMEEVT_CTR_SIZE     2
#define QF_FSM_ACTIVE

/* maximum # active objects--must match EXACTLY the QF_active[] definition  */
#define QF_MAX_ACTIVE           2

                               /* interrupt locking policy for IAR compiler */
#define QF_INT_LOCK()           __disable_interrupt()
#define QF_INT_UNLOCK()         __enable_interrupt()
/* #define QF_ISR_NEST */                    /* nesting of ISRs not allowed */

                                         /* interrupt entry and exit for QK */
#define QK_ISR_ENTRY()          ((void)0)
#define QK_ISR_EXIT()           QK_SCHEDULE_()

#include <intrinsics.h>  /* contains prototypes for the intrinsic functions */
#include <stdint.h>    /* Exact-width integer types. WG14/N843 C99 Standard */
#include "qepn.h"         /* QEP-nano platform-independent public interface */
#include "qfn.h"           /* QF-nano platform-independent public interface */
#include "qkn.h"           /* QK-nano platform-independent public interface */

#endif                                                        /* qpn_port_h */
