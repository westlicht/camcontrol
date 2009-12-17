/*****************************************************************************
* PELICAN crossing example
* Last Updated for Version: 4.0.00
* Date of the Last Update:  May 02, 2008
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
#ifndef __BSP_H__
#define __BSP_H__

/* CPU frequency */
#define F_CPU				16000000UL

/* Ticks per second */
#define BSP_TICKS_PER_SEC	50

#define TICKS(_ms_) ((_ms_) / (1000UL / BSP_TICKS_PER_SEC))

void bsp_init(void);

#endif // __BSP_H__
