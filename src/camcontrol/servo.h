/** @file servo.h
 *
 * Servo active object. Handles movement of the servos.
 *
 * @author Simon Kallweit, simon@weirdsoft.ch
 */

#ifndef __SERVO_H__
#define __SERVO_H__

extern struct servo_ao servo_ao;

void servo_ctor(void);
void servo_move(float x, float y);

#endif // __SERVO_H__
