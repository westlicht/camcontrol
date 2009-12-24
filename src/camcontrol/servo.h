/** @file servo.h
 *
 * Servo active object. Handles movement of the servos.
 *
 * @author Simon Kallweit, simon@weirdsoft.ch
 */

#ifndef __SERVO_H__
#define __SERVO_H__

#include "vector2.h"

extern struct servo_ao servo_ao;

void servo_ctor(void);
void servo_move(vec2f_t *v);

#endif // __SERVO_H__
