/**
 * Servo control using 16-bit PWM (timer1).
 */

#ifndef __SERVO_H__
#define __SERVO_H__

extern struct servo_ao servo_ao;

void servo_ctor(void);
void servo_set_pos(int servo, float pos);

#endif // __SERVO_H__
