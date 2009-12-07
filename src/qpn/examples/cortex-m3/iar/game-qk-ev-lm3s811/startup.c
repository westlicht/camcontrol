/*****************************************************************************
* Product: Startup code and Interrupt Vector Table for Cortex-M3, QK kernel
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

#pragma language=extended

/*****************************************************************************
* The following are constructs created by the linker, indicating where the
* the "DATA" and "BSS" segments reside in memory.  The initializers for the
* for the "data" segment resides immediately following the "text" segment.
*/
#pragma segment="CSTACK"

/* All interrupt hanlder functions used in the application .................*/
void __iar_program_start(void);

void ISR_SysTick(void);
void ISR_ADC(void);
void ISR_GPIOA(void);
void ISR_Nmi(void);
void ISR_Fault(void);
void ISR_DefaultHandler(void);

void QK_SVCall(void);                        /* QK SVCall exception handler */
void QK_PendSV(void);                        /* QK PendSV exception handler */

/*****************************************************************************
* Reserve space for the system stack.
*/
static unsigned long stack[150] @ ".noinit";

/*****************************************************************************
* The Interrupt Vector Table is normally located at address 0.
* When debugging in RAM, it can be located in RAM, aligned to at least 2^6.
* If you need to define interrupt service routines, make a copy of this
* file and include it in your project. The name "__vector_table" has special
* meaning for C-SPY: it is where the SP start value is found, and the NVIC
* vector table register (VTOR) is initialized to this address if != 0.
*/
typedef void (*ISR)(void);
typedef union {
    ISR hanlder;
    unsigned long mainStack;
} VectorTableEntry;

__root VectorTableEntry const __vector_table[] @ ".intvec" = {
                                          /* the initial stack pointer      */
    { .mainStack = (unsigned long)stack + sizeof(stack) },

    &__iar_program_start,                 /* The reset handler              */
    &ISR_Nmi,
    &ISR_Fault,
    &ISR_DefaultHandler,                  /* The MPU fault handler          */
    &ISR_DefaultHandler,                  /* The bus fault handler          */
    &ISR_DefaultHandler,                  /* The usage fault handler        */
    (ISR)0,                               /* Reserved                       */
    (ISR)0,                               /* Reserved                       */
    (ISR)0,                               /* Reserved                       */
    (ISR)0,                               /* Reserved                       */
    &QK_SVCall,                           /* SVCall handler for QK          */
    &ISR_DefaultHandler,                  /* Debug monitor handler          */
    (ISR)0,                               /* Reserved                       */
    &QK_PendSV,                           /* The PendSV handler for QK      */
    &ISR_SysTick,                         /* The SysTick handler            */
    &ISR_GPIOA,                           /* GPIO Port A                    */
    &ISR_DefaultHandler,                  /* GPIO Port B                    */
    &ISR_DefaultHandler,                  /* GPIO Port C                    */
    &ISR_DefaultHandler,                  /* GPIO Port D                    */
    &ISR_DefaultHandler,                  /* GPIO Port E                    */
    &ISR_DefaultHandler,                  /* UART0 Rx and Tx                */
    &ISR_DefaultHandler,                  /* UART1 Rx and Tx                */
    &ISR_DefaultHandler,                  /* SSI Rx and Tx                  */
    &ISR_DefaultHandler,                  /* I2C Master and Slave           */
    &ISR_DefaultHandler,                  /* PWM Fault                      */
    &ISR_DefaultHandler,                  /* PWM Generator 0                */
    &ISR_DefaultHandler,                  /* PWM Generator 1                */
    &ISR_DefaultHandler,                  /* PWM Generator 2                */
    &ISR_DefaultHandler,                  /* Quadrature Encoder             */
    &ISR_DefaultHandler,                  /* ADC Sequence 0                 */
    &ISR_DefaultHandler,                  /* ADC Sequence 1                 */
    &ISR_DefaultHandler,                  /* ADC Sequence 2                 */
    &ISR_ADC,                             /* ADC Sequence 3                 */
    &ISR_DefaultHandler,                  /* Watchdog timer                 */
    &ISR_DefaultHandler,                  /* Timer 0 subtimer A             */
    &ISR_DefaultHandler,                  /* Timer 0 subtimer B             */
    &ISR_DefaultHandler,                  /* Timer 1 subtimer A             */
    &ISR_DefaultHandler,                  /* Timer 1 subtimer B             */
    &ISR_DefaultHandler,                  /* Timer 2 subtimer A             */
    &ISR_DefaultHandler,                  /* Timer 2 subtimer B             */
    &ISR_DefaultHandler,                  /* Analog Comparator 0            */
    &ISR_DefaultHandler,                  /* Analog Comparator 1            */
    &ISR_DefaultHandler,                  /* Analog Comparator 2            */
    &ISR_DefaultHandler,                  /* System Control (PLL, OSC, BO)  */
    &ISR_DefaultHandler                   /* FLASH Control                  */
};
