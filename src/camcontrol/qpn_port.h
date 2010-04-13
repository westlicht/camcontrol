/** @file qpn_port.h
 *
 * QP-nano port. Adapted from the Quantum Leaps examples.
 *
 * @author Simon Kallweit, simon@weirdsoft.ch
 * @author Quantum Leaps
 */

#ifndef __QPN_PORT_H__
#define __QPN_PORT_H__

//#define Q_ROM
#define Q_ROM                   PROGMEM
#define Q_ROM_BYTE(rom_var_)    pgm_read_byte_near(&(rom_var_))
#define Q_ROM_PTR(rom_var_)     pgm_read_word_near(&(rom_var_))

#define Q_NFSM                  1
#define Q_PARAM_SIZE            2
#define QF_TIMEEVT_CTR_SIZE     2

/* maximum # active objects--must match EXACTLY the QF_active[] definition  */
#define QF_MAX_ACTIVE           5

                               /* interrupt locking policy for IAR compiler */
#define QF_INT_LOCK()           cli()
#define QF_INT_UNLOCK()         sei()

                            /* interrupt locking policy for interrupt level */
/* #define QF_ISR_NEST */                    /* nesting of ISRs not allowed */

#include <avr/io.h>
#include <avr/interrupt.h>                                   /* cli()/sei() */
#include <avr/pgmspace.h> /* accessing data in the program memory (PROGMEM) */

#include <stdint.h>    /* Exact-width integer types. WG14/N843 C99 Standard */
#include "qepn.h"         /* QEP-nano platform-independent public interface */
#include "qfn.h"           /* QF-nano platform-independent public interface */

#endif  // __QPN_PORT_H__
