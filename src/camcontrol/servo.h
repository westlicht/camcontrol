/**
 * Servo control using 16-bit PWM (timer1).
 */

#ifndef __SERVO_H__
#define __SERVO_H__

void servo_init(void);
void servo_set_pos(int servo, float pos);

#endif // __SERVO_H__
