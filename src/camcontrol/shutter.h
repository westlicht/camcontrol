/** @file shutter.h
 *
 * Shutter active object. Handles the cameras shutter. Implements different
 * behaviour for the different shutter modes.
 *
 * @author Simon Kallweit, simon@weirdsoft.ch
 */

#ifndef __SHUTTER_H__
#define __SHUTTER_H__

extern struct shutter_ao shutter_ao;

void shutter_ctor(void);

#endif // __SHUTTER_H__
