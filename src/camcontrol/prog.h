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
	vec2f_t fov;		/**< Field of view */
};

/** Spherical pan info */
struct spherical_info {
	vec2f_t fov;		/**< Field of view */
	uint16_t tiles;		/**< Total number of tiles */
	float origin;
	vec2f_t step;
	uint16_t rows;
	uint16_t cols;
	uint16_t row;
	uint16_t col;
	vec2f_t pos;
};

/** Giga pan info */
struct giga_info {
	vec2i_t tiles;
	vec2f_t origin;
	vec2f_t step;
	vec2i_t index;
};

extern struct prog_ao prog_ao;
extern struct camera_info camera_info;
extern struct spherical_info spherical_info;
extern struct giga_info giga_info;

void prog_ctor(void);

int prog_init_spherical_pan(void);
int prog_init_giga_pan(void);

#endif // __PROG_H__
