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
#include "qpn_port.h"
#include "game.h"
#include "bsp.h"

/* Local-scope objects -----------------------------------------------------*/
static void interrupt (*l_dosTmrISR)(void);
static void interrupt (*l_dosKbdISR)(void);

#define TMR_VECTOR      0x08
#define KBD_VECTOR      0x09

/*..........................................................................*/
static void interrupt tmrISR(void) {

    QF_tick();                             /* process all armed time events */

    QActive_postISR((QActive *)&AO_Tunnel,  TIME_TICK_SIG, 0);
    QActive_postISR((QActive *)&AO_Ship,    TIME_TICK_SIG, 0);
    QActive_postISR((QActive *)&AO_Missile, TIME_TICK_SIG, 0);

    outportb(0x20, 0x20);                    /* write EOI to the master PIC */
}
/*..........................................................................*/
static void interrupt kbdISR() {
    static uint8_t ship_pos = GAME_SHIP_Y;
    uint8_t key = inport(0x60);   /* key scan code from 8042 kbd controller */
    uint8_t kcr = inport(0x61);            /* get keyboard control register */

    outportb(0x61, (uint8_t)(kcr | 0x80));   /* toggle acknowledge bit high */
    outportb(0x61, kcr);                      /* toggle acknowledge bit low */

    switch (key) {
        case 200:                                               /* Up-arrow */
        case 208: {                                           /* Down-arrow */
            if ((key == (uint8_t)200) && (ship_pos > 0x00)) {
                --ship_pos;
            }
            else if ((key == (uint8_t)208)
                     && (ship_pos < (GAME_SCREEN_HEIGHT - 3))) {
                ++ship_pos;
            }

            QActive_postISR((QActive *)&AO_Ship, PLAYER_SHIP_MOVE_SIG,
                            ((ship_pos << 8) | GAME_SHIP_X));

            Video_printNumAt(24, 24, VIDEO_FGND_YELLOW, ship_pos);
            break;
        }
        case 57: {                                                 /* Space */
            static uint16_t ntrig = 0;

            QActive_postISR((QActive *)&AO_Ship,   PLAYER_TRIGGER_SIG, 0);
            QActive_postISR((QActive *)&AO_Tunnel, PLAYER_TRIGGER_SIG, 0);

            Video_printNumAt(47, 24, VIDEO_FGND_YELLOW, ++ntrig);
            break;
        }                                                            /* Esc */
        case 129: {
            QActive_postISR((QActive *)&AO_Tunnel, PLAYER_QUIT_SIG, 0);
            break;
        }
    }

    outportb(0x20, 0x20);                    /* write EOI to the master PIC */
}
/*..........................................................................*/
void BSP_init(void) {
    Video_clearScreen(VIDEO_BGND_LIGHT_GRAY);
    Video_clearRect( 0,  0, 80,   1, VIDEO_BGND_RED | VIDEO_BLINK);
    Video_clearRect( 0,  8, 80,  24, VIDEO_BGND_BLACK | VIDEO_FGND_WHITE);
    Video_clearRect( 0,  7, 80,   8, VIDEO_BGND_BLUE);
    Video_clearRect( 0, 24, 80,  25, VIDEO_BGND_BLUE);

    Video_clearRect(24, 24, 28,  25, VIDEO_BGND_RED | VIDEO_BLINK);
    Video_clearRect(24, 24, 28,  25, VIDEO_BGND_RED | VIDEO_BLINK);

    Video_printStrAt(30,  0, VIDEO_FGND_WHITE, "FLY 'n' SHOOT");
    Video_printStrAt(15,  2, VIDEO_FGND_BLACK,
                     "Press UP-arrow   to move the space ship up");
    Video_printStrAt(15,  3, VIDEO_FGND_BLACK,
                     "Press DOWN-arrow to move the space ship down");
    Video_printStrAt(15,  4, VIDEO_FGND_BLACK,
                     "Press SPACE      to fire the missile");
    Video_printStrAt(15,  5, VIDEO_FGND_BLACK,
                     "Press ESC        to quit the game");
    Video_printStrAt( 8, 24, VIDEO_FGND_WHITE, "Ship Position:");
    Video_printStrAt(37, 24, VIDEO_FGND_WHITE, "Triggers:");
    Video_printStrAt(61, 24, VIDEO_FGND_WHITE, "Score:");

    Video_clearRect(24, 24, 28,  25, VIDEO_BGND_RED);
    Video_clearRect(47, 24, 51,  25, VIDEO_BGND_RED);
    Video_clearRect(68, 24, 72,  25, VIDEO_BGND_RED);
    Video_printNumAt(24, 24, VIDEO_FGND_YELLOW, 0);
    Video_printNumAt(47, 24, VIDEO_FGND_YELLOW, 0);
    Video_printNumAt(68, 24, VIDEO_FGND_YELLOW, 0);
}
/*..........................................................................*/
void BSP_drawBitmap(uint8_t const *bitmap, uint8_t width, uint8_t height) {
    Video_drawBitmapAt(0, 8, bitmap, width, height);
}
/*..........................................................................*/
void BSP_drawNString(uint8_t x, uint8_t y, char const *str) {
    Video_drawStringAt(x, 8 + y*8, str);
}
/*..........................................................................*/
void BSP_updateScore(uint16_t score) {
    if (score == 0) {
        Video_clearRect(68, 24, 72,  25, VIDEO_BGND_RED);
    }
    Video_printNumAt(68, 24, VIDEO_FGND_YELLOW, score);
}

/*..........................................................................*/
void QF_onStartup(void) {
                                      /* save the origingal DOS vectors ... */
    l_dosTmrISR   = getvect(TMR_VECTOR);
    l_dosKbdISR   = getvect(KBD_VECTOR);

    QF_INT_LOCK();
    setvect(TMR_VECTOR, &tmrISR);
    setvect(KBD_VECTOR, &kbdISR);
    QF_INT_UNLOCK();
}
/*..........................................................................*/
void QF_stop(void) {
                                    /* restore the original DOS vectors ... */
    if (l_dosTmrISR != (void interrupt (*)(void))0) { /* DOS vectors saved? */
        QF_INT_LOCK();
        setvect(TMR_VECTOR, l_dosTmrISR);
        setvect(KBD_VECTOR, l_dosKbdISR);
        QF_INT_UNLOCK();
    }
                                                       /* clear the display */
    //Video_clearScreen(VIDEO_BGND_BLACK | VIDEO_FGND_LIGHT_GRAY);
    _exit(0);                                                /* exit to DOS */
}
/*..........................................................................*/
void QF_onIdle(void) {                                        /* see NOTE01 */
    QF_INT_UNLOCK();
}
/*--------------------------------------------------------------------------*/
void Q_onAssert(char const Q_ROM * const Q_ROM_VAR file, int line) {
    Video_clearRect(0, 24, 80, 25, VIDEO_BGND_RED);
    Video_printStrAt(0, 24, VIDEO_FGND_WHITE, "ASSERTION FAILED in file:");
    Video_printStrAt(26, 24, VIDEO_FGND_YELLOW, file);
    Video_printStrAt(57, 24, VIDEO_FGND_WHITE, "line:");
    Video_printNumAt(62, 24, VIDEO_FGND_YELLOW, line);

    QF_stop();                                       /* stop QF and cleanup */
}

/*****************************************************************************
* NOTE01:
* The QF_onIdle() callback is called with interrupts locked, because the
* determination of the idle condition might change by any interrupt posting
* an event. QF_onIdle() must internally unlock interrupts, ideally atomically
* with putting the CPU to the power-saving mode.
*/
