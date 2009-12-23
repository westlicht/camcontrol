/** @file key.c
 *
 * Keyboard and rotary encoder handling.
 *
 * @author Simon Kallweit, simon@weirdsoft.ch
 */

#ifndef __KEY_H__
#define __KEY_H__

/* Keys */
#define KEY_UP		0
#define KEY_LEFT	1
#define KEY_ENTER	2
#define KEY_RIGHT	3
#define KEY_DOWN	4

/* Encoder */
#define ENC_UP		0
#define ENC_DOWN	1

void key_init(void);
void key_scan(void);

#endif // __KEY_H__
