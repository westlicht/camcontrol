/*****************************************************************************
* Product: PELICAN crossing example
* Last Updated for Version: 4.0.04
* Date of the Last Update:  Apr 09, 2009
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) 2002-2009 Quantum Leaps, LLC. All rights reserved.
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
#include "pelican.h"

#include "hw_ints.h"
#include "hw_memmap.h"
#include "hw_nvic.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/systick.h"
#include "driverlib/sysctl.h"

/*..........................................................................*/
void ISR_SysTick(void) {
    QF_tick();
}
/*..........................................................................*/
void ISR_GPIOA(void) {
    QActive_postISR((QActive *)&AO_Ped, PEDS_WAITING_SIG, 0);
}
/*..........................................................................*/
void ISR_Nmi(void) {
    for (;;) {                                   /* sit in an infinite loop */
    }
}
/*..........................................................................*/
void ISR_Fault(void) {
    for (;;) {                                   /* sit in an infinite loop */
    }
}
/*..........................................................................*/
void ISR_DefaultHandler(void) {
    for (;;) {                                   /* sit in an infinite loop */
    }
}

/*..........................................................................*/
void BSP_init(void) {
    /* Set the clocking to run directly from the crystal */
    SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN
                   | SYSCTL_XTAL_6MHZ);

    /* Enable the peripherals used by this example */
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);

    /* Configure the User LED (PortC pin5) */
    GPIODirModeSet(GPIO_PORTC_BASE, GPIO_PIN_5, GPIO_DIR_MODE_OUT);
    GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_5, 0);

    OSRAMInit(1);           /* Initialize the OLED display and write status */
}
/*..........................................................................*/
void QF_onStartup(void) {
    /* Set up and enable the SysTick timer.  It will be used as a reference
    * for delay loops in the interrupt handlers.  The SysTick timer period
    * will be set up for BSP_TICKS_PER_SEC.
    */
    SysTickPeriodSet(SysCtlClockGet() / BSP_TICKS_PER_SEC);
    SysTickEnable();
    IntPrioritySet(FAULT_SYSTICK, 0xC0);     /* set the priority of SysTick */
    SysTickIntEnable();                    /* Enable the SysTick interrupts */

    /* GPIOA interrupt is used for testing preemptions */
    IntEnable(INT_GPIOA);
    IntPrioritySet(INT_GPIOA, 0x80);
}
/*..........................................................................*/
void QF_onIdle(void) {        /* entered with interrupts LOCKED, see NOTE01 */

    /* toggle the User LED on and then off, see NOTE02 */
    GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_5, GPIO_PIN_5);   /* User LED on */
    GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_5, 0);           /* User LED off */

#ifdef NDEBUG
    /* put the CPU and peripherals to the low-power mode, see NOTE02 */
    __asm("WFI");
#endif
    QF_INT_UNLOCK();                            /* always unlock interrupts */
}
/*..........................................................................*/
/* error routine that is called if the Luminary library encounters an error */
void __error__(char *pcFilename, unsigned long ulLine) {
    Q_onAssert(pcFilename, ulLine);
}
/*..........................................................................*/
void Q_onAssert(char const Q_ROM * const Q_ROM_VAR file, int line) {
    (void)file;                                   /* avoid compiler warning */
    (void)line;                                   /* avoid compiler warning */
    QF_INT_LOCK();            /* make sure that all interrupts are disabled */
    for (;;) {       /* NOTE: replace the loop with reset for final version */
    }
}
/*..........................................................................*/
void BSP_signalCars(enum BSP_CarsSignal sig) {
    switch (sig) {
        case CARS_RED: {
            OSRAMStringDraw("RED", 78, 1);
            break;
        }
        case CARS_YELLOW: {
            OSRAMStringDraw("YLW", 78, 1);
            break;
        }
        case CARS_GREEN: {
            OSRAMStringDraw("GRN", 78, 1);
            break;
        }
        case CARS_OFF: {
            OSRAMStringDraw("   ", 78, 1);
            break;
        }
    }
}
/*..........................................................................*/
void BSP_signalPeds(enum BSP_PedsSignal sig) {
    switch (sig) {
        case PEDS_DONT_WALK: {
            OSRAMStringDraw("DON'T WALK", 0, 1);
            break;
        }
        case PEDS_BLANK: {
            OSRAMStringDraw("          ", 0, 1);
            break;
        }
        case PEDS_WALK: {
            OSRAMStringDraw("** WALK **", 0, 1);
            break;
        }
    }
}
/*..........................................................................*/
void BSP_showState(uint8_t prio, char const *state) {
    if (QF_active[prio].act == (QActive *)&AO_Pelican) {
        OSRAMStringDraw(state, 0, 0);
    }
}

#define BSP_showState(act_, state_) \

/*****************************************************************************
* NOTE01:
* The QF_onIdle() callback is called with interrupts locked, because the
* determination of the idle condition might change by any interrupt posting
* an event. QF_onIdle() must internally unlock interrupts, ideally atomically
* with putting the CPU to the power-saving mode.
*
* NOTE02:
* The User LED is used to visualize the idle loop activity. The brightness
* of the LED is proportional to the frequency of invcations of the idle loop.
* Please note that the LED is toggled with interrupts locked, so no interrupt
* execution time contributes to the brightness of the User LED.
*
*/
