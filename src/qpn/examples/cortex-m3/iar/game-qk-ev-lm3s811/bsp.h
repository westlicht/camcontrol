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
#ifndef bsp_h
#define bsp_h

#include "hw_types.h"                                       /* for tBoolean */
#include "osram96x16x1.h"                                    /* OLED driver */

#define BSP_TICKS_PER_SEC    30
#define BSP_SCREEN_WIDTH     96
#define BSP_SCREEN_HEIGHT    16

void BSP_init(void);

void BSP_drawBitmap(uint8_t const *bitmap,
                    uint8_t width,                             /* in pixels */
                    uint8_t height);  /* in bytes of pixels (8-pixesl/byte) */
void BSP_drawNString(uint8_t x,                              /* x in pixels */
                     uint8_t y,                      /* y position in chars */
                     char const *str);
void BSP_updateScore(uint16_t score);     /* update the score on the margin */

#endif                                                             /* bsp_h */
