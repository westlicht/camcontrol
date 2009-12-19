/**
 * Handles keys and encoder.
 * Keys are scanned using TIMER1.
 */

#ifndef __KEY_H__
#define __KEY_H__

#define KEY_UP		0
#define KEY_LEFT	1
#define KEY_ENTER	2
#define KEY_RIGHT	3
#define KEY_DOWN	4

#define ENC_UP		0
#define ENC_DOWN	1

void key_init(void);

#endif // __KEY_H__
