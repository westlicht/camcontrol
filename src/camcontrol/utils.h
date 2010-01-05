/** @file utils.h
 *
 * Some utility functions.
 *
 * @author Simon Kallweit, simon@weirdsoft.ch
 */

#ifndef __UTILS_H__
#define __UTILS_H__

#include <math.h>

#define M_2PI (M_PI * 2.0)

#define CLAMP(_val_, _min_, _max_) \
	((_val_) < (_min_) ? (_min_) : ((_val_) > (_max_) ? (_max_) : (_val_)))

#define ABS(_val_) \
	((_val_) < 0 ? -(_val_) : (_val_))

#define MIN(_a_, _b_) \
	((_a_) < (_b_) ? (_a_) : (_b_))

#define MAX(_a_, _b_) \
	((_a_) > (_b_) ? (_a_) : (_b_))


float rad2deg(float rad);
float deg2rad(float rad);

#endif // __UTILS_H__
