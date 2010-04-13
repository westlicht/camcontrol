/** @file bsp.h
 *
 * Board support package. Adapted from quantum leaps examples.
 *
 * @author Simon Kallweit, simon@weirdsoft.ch
 * @author Quantum Leaps
 */

#include <avr/io.h>
#include <avr/interrupt.h>

#include "qpn_port.h"
#include "bsp.h"
#include "lcd.h"
#include "key.h"
#include "debug.h"
#include "defines.h"

/**
 * Initializes the board. Sets ports.
 */
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

/**
 * Timer0 interrupt. Used for keyboard scanning and system ticking.
 */
ISR(TIMER0_COMP_vect)
{
    static uint8_t counter;

    key_scan();
    if (++counter >= BSP_MS_PER_TICK) {
        QF_tick();
        counter = 0;
    }
}

void QF_onStartup(void)
{
    cli();

    // Timer0 is used both for key scanning as well as system ticks
    // Set the output compare value (tick once per millisecond)
    OCR0  = ((F_CPU / 1024) / 1000 - 1);
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
    on_assert(file, line);
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
