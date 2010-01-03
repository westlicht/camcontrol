/** @file vector2.h
 *
 * Defines 2d vector types.
 *
 * @author Simon Kallweit, simon@weirdsoft.ch
 */

#ifndef __VECTOR2_H__
#define __VECTOR2_H__

/** Float vector */
typedef struct {
	float x, y;
} vec2f_t;

/** Integer vector */
typedef struct {
	int16_t x, y;
} vec2i_t;

/**
 * Sets a vector.
 * @param _vec_ Vector pointer
 * @param _x_ X
 * @param _y_ Y
 */
#define vec2(_vec_, _x_, _y_)		\
	do {							\
		(_vec_)->x = _x_;			\
		(_vec_)->y = _y_;			\
	} while (0)

#endif // __VECTOR2_H__
