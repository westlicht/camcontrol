/** @file key.c
 *
 * Keyboard and rotary encoder handling.
 *
 * @author Simon Kallweit, simon@weirdsoft.ch
 */

#ifndef __KEY_H__
#define __KEY_H__

/* Keys */
enum keys {
	KEY_UP,
	KEY_LEFT,
	KEY_ENTER,
	KEY_RIGHT,
	KEY_DOWN,
};

/* Encoder directions */
enum enc_dirs {
	ENC_UP,
	ENC_DOWN,
};

void key_init(void);
void key_scan(void);

#endif // __KEY_H__
