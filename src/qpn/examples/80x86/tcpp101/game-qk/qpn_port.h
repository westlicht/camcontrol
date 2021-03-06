/*****************************************************************************
* Product: "Fly 'n' Shoot" game example with QK-nano
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

#define Q_PARAM_SIZE            4
#define Q_NFSM
#define QF_TIMEEVT_CTR_SIZE     2

/* maximum # active objects--must match EXACTLY the QF_active[] definition  */
#define QF_MAX_ACTIVE           3

                                 /* interrupt locking policy for task level */
#define QF_INT_LOCK()           disable()
#define QF_INT_UNLOCK()         enable()

                            /* interrupt locking policy for interrupt level */
#define QF_ISR_NEST

                                        /* interrupt entry/exit for QK-nano */
#ifndef QF_ISR_NEST                              /* ISR nesting NOT allowed */
    #define QK_ISR_ENTRY()      ((void)0)
    #define QK_ISR_EXIT()  do { \
        outportb(0x20, 0x20); \
        QK_SCHEDULE_(); \
    } while (0)
#else                                                /* ISR nesting allowed */
    #define QK_ISR_ENTRY() do { \
        ++QK_intNest_; \
        enable(); \
    } while (0)
    #define QK_ISR_EXIT()  do { \
        disable(); \
        outportb(0x20, 0x20); \
        --QK_intNest_; \
        QK_SCHEDULE_(); \
    } while (0)
#endif


/* Exact-width types. WG14/N843 C99 Standard, Section 7.18.1.1 */
typedef signed   char  int8_t;
typedef signed   int   int16_t;
typedef signed   long  int32_t;
typedef unsigned char  uint8_t;
typedef unsigned int   uint16_t;
typedef unsigned long  uint32_t;

#include <dos.h>                                                 /* DOS API */
#undef outportb /*don't use the macro because it has a bug in Turbo C++ 1.01*/

#include "qepn.h"              /* QEP-nano platform-independent header file */
#include "qfn.h"                /* QF-nano platform-independent header file */
#include "qkn.h"                /* QK-nano platform-independent header file */

#endif                                                        /* qpn_port_h */
