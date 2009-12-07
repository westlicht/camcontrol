/*****************************************************************************
* PELICAN crossing example with QK-nano preemptive kernel
* Last Updated for Version: 4.0.01
* Date of the Last Update:  Jun 24, 2008
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

#define LED_ON(num_)       (PORTD |= (1 << (num_)))
#define LED_OFF(num_)      (PORTD &= ~(1 << (num_)))
#define LED_ON_ALL()       (PORTD = 0xFF)
#define LED_OFF_ALL()      (PORTD = 0x00)

/*..........................................................................*/
SIGNAL(SIG_OUTPUT_COMPARE0) {
    QK_ISR_ENTRY();                       /* inform QK-nano about ISR entry */
    QF_tick();
    QK_ISR_EXIT();                         /* inform QK-nano about ISR exit */
}
/*..........................................................................*/
void BSP_init(void) {
    DDRD  = 0xFF;                    /* All PORTD pins are outputs for LEDs */
    LED_OFF_ALL();                                     /* trun off all LEDs */

    /* set the output compare value */
    OCR0A  = ((F_CPU / BSP_TICKS_PER_SEC / 1024) - 1);
}
/*..........................................................................*/
void QF_onStartup(void) {
    cli();                    /* make sure that all interrupts are disabled */

    /* set Timer0 in CTC mode, 1/1024 prescaler, start the timer ticking */
    TCCR0A = ((1 << WGM01) | (0 << WGM00) | (5 << CS00));
    TIMSK0 = (1 << OCIE0A);              /* Enable TIMER0 compare Interrupt */

    sei();                     /* make sure that all interrupts are enabled */
}
/*..........................................................................*/
void QK_onIdle(void) {

                     /* toggle the LED number 7 on and then off, see NOTE01 */
    QF_INT_LOCK();
    LED_ON(7);
    LED_OFF(7);
    QF_INT_UNLOCK();

#ifdef NDEBUG
    SMCR = (0 << SM0) | (1 << SE);/*idle sleep mode, adjust to your project */
    __asm__ __volatile__ ("sleep" "\n\t" :: );

    SMCR = 0;                                           /* clear the SE bit */
#endif
}
/*..........................................................................*/
void Q_onAssert(char const Q_ROM * const Q_ROM_VAR file, int line) {
    (void)file;                                   /* avoid compiler warning */
    (void)line;                                   /* avoid compiler warning */
    cli();                                           /* lock all interrupts */
    LED_ON_ALL();                                            /* all LEDs on */
    for (;;) {       /* NOTE: replace the loop with reset for final version */
    }
}
/*..........................................................................*/
void BSP_signalCars(enum BSP_CarsSignal sig) {
    switch (sig) {
        case CARS_RED:       LED_ON(0);  LED_OFF(1); LED_OFF(2); break;
        case CARS_YELLOW:    LED_OFF(0); LED_ON(1);  LED_OFF(2); break;
        case CARS_GREEN:     LED_OFF(0); LED_OFF(1); LED_ON(2);  break;
        case CARS_OFF:       LED_OFF(0); LED_OFF(1); LED_OFF(2); break;
    }
}
/*..........................................................................*/
void BSP_signalPeds(enum BSP_PedsSignal sig) {
    switch (sig) {
        case PEDS_DONT_WALK: LED_ON(4);  LED_OFF(5); break;
        case PEDS_BLANK:     LED_OFF(4); LED_OFF(5); break;
        case PEDS_WALK:      LED_OFF(4); LED_ON(5);  break;
    }
}

/*****************************************************************************
* NOTE01:
* The LED[7] is used to visualize the idle loop activity. The brightness
* of the LED is proportional to the frequency of invcations of the idle loop.
* Please note that the LED is toggled with interrupts locked, so no interrupt
* execution time contributes to the brightness of the User LED.
*/
