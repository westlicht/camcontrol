/*****************************************************************************
* Product: "Fly 'n' Shoot" game example with QK
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
#include "game.h"
#include "bsp.h"
#include "qassert.h"

#include "hw_ints.h"
#include "hw_memmap.h"
#include "hw_nvic.h"
#include "driverlib/adc.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/timer.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"

//Q_DEFINE_THIS_FILE

/* Local-scope objects -----------------------------------------------------*/
#define PUSH_BUTTON             GPIO_PIN_4
#define USER_LED                GPIO_PIN_5

/*..........................................................................*/
void ISR_SysTick(void) {
    QK_ISR_ENTRY();                       /* inform QK-nano about ISR entry */

    QF_tick();                             /* process all armed time events */

    QActive_postISR((QActive *)&AO_Tunnel,  TIME_TICK_SIG, 0);
    QActive_postISR((QActive *)&AO_Ship,    TIME_TICK_SIG, 0);
    QActive_postISR((QActive *)&AO_Missile, TIME_TICK_SIG, 0);

    QK_ISR_EXIT();                         /* inform QK-nano about ISR exit */
}
/*..........................................................................*/
void ISR_ADC(void) {
    static uint32_t adcLPS = 0;            /* Low-Pass-Filtered ADC reading */
    static uint32_t wheel = 0;                   /* the last wheel position */

    static uint32_t btn_debounced  = 0;
    static uint8_t  debounce_state = 0;

    unsigned long tmp;

    QK_ISR_ENTRY();                       /* inform QK-nano about ISR entry */

    ADCIntClear(ADC_BASE, 3);                    /* clear the ADC interrupt */
    ADCSequenceDataGet(ADC_BASE, 3, &tmp);    /* read the data from the ADC */

    /* 1st order low-pass filter: time constant ~= 2^n samples
     * TF = (1/2^n)/(z-((2^n - 1)/2^n)),
     * eg, n=3, y(k+1) = y(k) - y(k)/8 + x(k)/8 => y += (x - y)/8
     */
    adcLPS += (((int)tmp - (int)adcLPS + 4) >> 3);       /* Low-Pass-Filter */

    /* compute the next position of the wheel */
    tmp = (((1 << 10) - adcLPS)*(BSP_SCREEN_HEIGHT - 2)) >> 10;

    if (tmp != wheel) {                   /* did the wheel position change? */
        QActive_postISR((QActive *)&AO_Ship, PLAYER_SHIP_MOVE_SIG,
                        ((tmp << 8) | GAME_SHIP_X));
        wheel = tmp;                 /* save the last position of the wheel */
    }

    tmp = GPIOPinRead(GPIO_PORTC_BASE, PUSH_BUTTON);   /* read the push btn */
    switch (debounce_state) {
        case 0:
            if (tmp != btn_debounced) {
                debounce_state = 1;         /* transition to the next state */
            }
            break;
        case 1:
            if (tmp != btn_debounced) {
                debounce_state = 2;         /* transition to the next state */
            }
            else {
                debounce_state = 0;           /* transition back to state 0 */
            }
            break;
        case 2:
            if (tmp != btn_debounced) {
                debounce_state = 3;         /* transition to the next state */
            }
            else {
                debounce_state = 0;           /* transition back to state 0 */
            }
            break;
        case 3:
            if (tmp != btn_debounced) {
                btn_debounced = tmp;     /* save the debounced button value */

                if (tmp == 0) {                 /* is the button depressed? */
                    QActive_postISR((QActive *)&AO_Ship,
                                    PLAYER_TRIGGER_SIG, 0);
                    QActive_postISR((QActive *)&AO_Tunnel,
                                    PLAYER_TRIGGER_SIG, 0);
                }
            }
            debounce_state = 0;               /* transition back to state 0 */
            break;
    }

    QK_ISR_EXIT();                         /* inform QK-nano about ISR exit */
}
/*..........................................................................*/
void ISR_GPIOA(void) {
    QK_ISR_ENTRY();                       /* inform QK-nano about ISR entry */
    QActive_postISR((QActive *)&AO_Tunnel, TAKE_OFF_SIG, 0); /* for testing */
    QK_ISR_EXIT();                         /* inform QK-nano about ISR exit */
}
/*..........................................................................*/
void ISR_Nmi(void) {
    QK_ISR_ENTRY();                       /* inform QK-nano about ISR entry */
    for (;;) {                                   /* sit in an infinite loop */
    }
    QK_ISR_EXIT();                         /* inform QK-nano about ISR exit */
}
/*..........................................................................*/
void ISR_Fault(void) {
    QK_ISR_ENTRY();                       /* inform QK-nano about ISR entry */
    for (;;) {                                   /* sit in an infinite loop */
    }
    QK_ISR_EXIT();                         /* inform QK-nano about ISR exit */
}
/*..........................................................................*/
void ISR_DefaultHandler(void) {
    QK_ISR_ENTRY();                       /* inform QK-nano about ISR entry */
    for (;;) {                                   /* sit in an infinite loop */
    }
    QK_ISR_EXIT();                         /* inform QK-nano about ISR exit */
}

/*..........................................................................*/
void BSP_init(void) {
    /* Set the clocking to run at 20MHz from the PLL. */
    SysCtlClockSet(SYSCTL_SYSDIV_10  | SYSCTL_USE_PLL
                   | SYSCTL_OSC_MAIN | SYSCTL_XTAL_6MHZ);

    /* Enable the peripherals used by the application. */
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);

    /* Configure the ADC to sample the potentiometer when the timer expires.
    * After sampling, the ADC will interrupt the processor
    */
    ADCSequenceConfigure(ADC_BASE, 3, ADC_TRIGGER_TIMER, 0);
    ADCSequenceStepConfigure(ADC_BASE, 3, 0,
                             ADC_CTL_CH0 | ADC_CTL_IE | ADC_CTL_END);
    ADCSequenceEnable(ADC_BASE, 3);

    /* Configure the timer to generate triggers to the ADC to sample the
    * potentiometer.
    */
    TimerConfigure(TIMER1_BASE, TIMER_CFG_32_BIT_PER);
    TimerLoadSet(TIMER1_BASE, TIMER_A, SysCtlClockGet() / 120);
    TimerControlStall(TIMER1_BASE, TIMER_A, true);
    TimerControlTrigger(TIMER1_BASE, TIMER_A, true);

    /* Configure the LED, push button, and UART GPIOs as required. */
    GPIODirModeSet(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1,
                   GPIO_DIR_MODE_HW);
    GPIODirModeSet(GPIO_PORTC_BASE, PUSH_BUTTON, GPIO_DIR_MODE_IN);
    GPIODirModeSet(GPIO_PORTC_BASE, USER_LED, GPIO_DIR_MODE_OUT);
    GPIOPinWrite(GPIO_PORTC_BASE, USER_LED, 0);

    /* Set up the SysTick timer, which is used as the system clock tick. */
    SysTickPeriodSet(SysCtlClockGet() / BSP_TICKS_PER_SEC);
    SysTickEnable();

    IntPrioritySet(INT_ADC3,      (5 << 5)); /* set the priority of ADC - 3 */
    IntPrioritySet(FAULT_SYSTICK, (6 << 5)); /* set the priority of SysTick */

    OSRAMInit(1);                      /* Initialize the OSRAM OLED display */

    QK_init();                             /* initialize the QK-nano kernel */
}
/*..........................................................................*/
void BSP_drawBitmap(uint8_t const *bitmap, uint8_t width, uint8_t height) {
    OSRAMImageDraw(bitmap, 0, 0, width, (height >> 3));
}
/*..........................................................................*/
void BSP_drawNString(uint8_t x, uint8_t y, char const *str) {
    OSRAMStringDraw(str, x, y);
}
/*..........................................................................*/
void BSP_updateScore(uint16_t score) {
    /* not available on the EV-LM3S811 board */
}

/*..........................................................................*/
void QF_onStartup(void) {               /* enable the configured interrupts */
    ADCIntEnable(ADC_BASE, 3);
    IntEnable(INT_ADC3);
    TimerEnable(TIMER1_BASE, TIMER_A);
    SysTickIntEnable();
}
/*..........................................................................*/
void QF_stop(void) {
}
/*..........................................................................*/
void QK_onIdle(void) {

    /* toggle the User LED on and then off, see NOTE02 */
    QF_INT_LOCK();
    GPIOPinWrite(GPIO_PORTC_BASE, USER_LED, USER_LED);       /* User LED on */
    GPIOPinWrite(GPIO_PORTC_BASE, USER_LED, 0);             /* User LED off */
    QF_INT_UNLOCK();

#ifdef NDEBUG
    /* Put the CPU and peripherals to the low-power mode.
    * you might need to customize the clock management for your application,
    * see the datasheet for your particular Cortex-M3 MCU.
    */
    __asm("WFI \n");                                  /* Wait-For-Interrupt */
#endif
}

/*..........................................................................*/
void Q_onAssert(char const Q_ROM * const Q_ROM_VAR file, int line) {
    (void)file;                                   /* avoid compiler warning */
    (void)line;                                   /* avoid compiler warning */
    QF_INT_LOCK();            /* make sure that all interrupts are disabled */
    for (;;) {       /* NOTE: replace the loop with reset for final version */
    }
}
/* error routine that is called if the Luminary library encounters an error */
void __error__(char *pcFilename, unsigned long ulLine) {
    Q_onAssert(pcFilename, ulLine);
}

/*****************************************************************************
* NOTE01:
* The User LED is used to visualize the idle loop activity. The brightness
* of the LED is proportional to the frequency of invcations of the idle loop.
* Please note that the LED is toggled with interrupts locked, so no interrupt
* execution time contributes to the brightness of the User LED.
*
*/
