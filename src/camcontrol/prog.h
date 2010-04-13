/** @file prog.h
 *
 * Program active object. Handles the overall behaviour of the different
 * camera control programs like, spherical pan, giga pan, timelapse etc.
 *
 * @author Simon Kallweit, simon@weirdsoft.ch
 */

#ifndef __PROG_H__
#define __PROG_H__

#include <stdint.h>
#include "vector2.h"

/** Programs */
enum programs {
    PROG_SPHERICAL_PAN,
    PROG_GIGA_PAN,
    PROG_TIMELAPSE,
};

/** Camera info */
struct camera_info {
    vec2f_t fov;            /**< Field of view */
};

/** Spherical pan info */
struct spherical_info {
    uint16_t total_tiles;   /**< Total number of tiles */
    float origin;           /**< Vertical origin of 2nd row */
    vec2i_t tiles;          /**< Number of tiles (rows/cols) */
    vec2f_t step;           /**< Step size (row/col) */
    vec2i_t index;          /**< Index (row/col) */
    vec2f_t pos;            /**< Camera position */
};

/** Giga pan info */
struct giga_info {
    vec2i_t tiles;          /**< Number of tiles (rows/cols) */
    vec2f_t origin;         /**< Origin */
    vec2f_t step;           /**< Step size (row/col) */
    vec2i_t index;          /**< Index (row/col) */
};

extern struct prog_ao prog_ao;
extern struct camera_info camera_info;
extern struct spherical_info spherical_info;
extern struct giga_info giga_info;

void prog_ctor(void);

int prog_init_spherical_pan(void);
int prog_init_giga_pan(void);

#endif // __PROG_H__
