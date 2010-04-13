/** @file bsp.h
 *
 * Board support package. Adapted from quantum leaps examples.
 *
 * @author Simon Kallweit, simon@weirdsoft.ch
 * @author Quantum Leaps
 */

#ifndef __BSP_H__
#define __BSP_H__

#define F_CPU               16000000UL      /**< CPU frequency */

#define BSP_TICKS_PER_SEC   50              /**< Ticks per second */

#define BSP_MS_PER_TICK     (1000UL / BSP_TICKS_PER_SEC)
#define TICKS(_ms_)         ((_ms_) / BSP_MS_PER_TICK)

void bsp_init(void);

#endif // __BSP_H__
