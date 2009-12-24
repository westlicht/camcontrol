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

/** Spherical pan parameters */
struct spherical_pan {
	vec2f_t fov;
	uint16_t tiles_x;
	uint16_t tiles_y;
};

/** Giga pan parameters */
struct giga_pan {
	vec2f_t origin;
	vec2f_t step;
	vec2i_t tiles;
	vec2i_t index;
};

extern struct prog_ao prog_ao;
extern struct spherical_pan spherical_pan;
extern struct giga_pan giga_pan;

void prog_ctor(void);

int prog_init_spherical_pan(void);
int prog_init_giga_pan(void);

#endif // __PROG_H__
