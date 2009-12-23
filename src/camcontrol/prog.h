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

/* Programs */
#define PROG_SPHERICAL_PAN	0
#define PROG_GIGA_PAN		1
#define PROG_TIMELAPSE		2

/** Spherical pan parameters */
struct spherical_pan {
	float fov_x;
	float fov_y;
	uint16_t tiles_x;
	uint16_t tiles_y;
};

/** Giga pan parameters */
struct giga_pan {

};

extern struct prog_ao prog_ao;
extern struct spherical_pan spherical_pan;

void prog_ctor(void);

void prog_init_spherical_pan(void);

#endif // __PROG_H__
