/*****************************************************************************
* PELICAN crossing example
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
#include <avr/io.h>
#include <avr/interrupt.h>

#include <string.h>
#include "qpn_port.h"
#include "bsp.h"
#include "lcd.h"
#include "debug.h"
#include "defines.h"



#define LED_ON(num_)       (PORTD |= (1 << (num_)))
#define LED_OFF(num_)      (PORTD &= ~(1 << (num_)))
#define LED_ON_ALL()       (PORTD = 0xFF)
#define LED_OFF_ALL()      (PORTD = 0x00)

void bsp_init(void)
{
	// Input/Output Ports initialization
	// Port A initialization
	PORTA = 0x00;
	DDRA = 0x40;

	// Port B initialization
	PORTB = 0x00;
	DDRB = 0x00;

	// Port C initialization
	PORTC = 0x00;
	DDRC = 0xF7;

	// Port D initialization
	PORTD = 0x00;
	DDRD = 0x08;

	// Port E initialization
	PORTE = 0x00;
	DDRE = 0x30;

	// Port F initialization
	PORTF = 0x00;
	DDRF = 0x00;

	// Port G initialization
	PORTG = 0x00;
	DDRG = 0x00;
}

ISR(TIMER0_COMP_vect)
{
	QF_tick();
}

void QF_onStartup(void)
{
	cli();

    // Set the output compare value
    OCR0  = ((F_CPU / 1024) / BSP_TICKS_PER_SEC - 1);
	// Set Timer0 in CTC mode, 1/1024 prescaler, start the timer ticking
	TCCR0 = ((1 << WGM01) | (0 << WGM00) | (7 << CS00));
	TIMSK |= _BV(OCIE0);

	sei();
}

void QF_onIdle(void)
{
	// Entered with interrupts LOCKED, see NOTE01

#ifdef NDEBUG
	SMCR = (0 << SM0) | (1 << SE);/*idle sleep mode, adjust to your project */

      /* never separate the following two assembly instructions, see NOTE03 */
    __asm__ __volatile__ ("sei" "\n\t" :: );
    __asm__ __volatile__ ("sleep" "\n\t" :: );

    SMCR = 0;                                           /* clear the SE bit */
#else
    QF_INT_UNLOCK();
#endif
}

void Q_onAssert(char const Q_ROM * const Q_ROM_VAR file, int line)
{
	char tmp[17];

	DBG_LED_ON();

	// Disable interrupts
    cli();

	strncpy_P(tmp, file, strlen(file));
	lcd_clear();
	lcd_write(0, 0, tmp);

	while (1);
}

/*****************************************************************************
* NOTE01:
* The QF_onIdle() callback is called with interrupts locked, because the
* determination of the idle condition might change by any interrupt posting
* an event. QF_onIdle() must internally unlock interrupts, ideally atomically
* with putting the CPU to the power-saving mode.
*
* NOTE03:
* As described in the "AVR Datasheet" in Section "Reset and Interrupt
* Handling", when using the SEI instruction to enable interrupts, the
* instruction following SEI will be executed before any pending interrupts.
* As the Datasheet shows in the assembly example, the pair of instructions
*     SEI       ; enable interrupts
*     SLEEP     ; go to the sleep mode
* executes ATOMICALLY, and so no interrupt can be serviced between these
* instructins. You should NEVER separate these two lines.
*/
