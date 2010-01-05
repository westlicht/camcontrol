/** @file utils.c
 *
 * Some utility functions.
 *
 * @author Simon Kallweit, simon@weirdsoft.ch
 */

#include "utils.h"

float rad2deg(float rad)
{
	return rad / M_PI * 180.0;
}

float deg2rad(float deg)
{
	return deg / 180.0 * M_PI;
}
