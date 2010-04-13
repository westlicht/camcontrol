/** @file backlight.h
 *
 * Backlight active object. Handles the lcd backlight.
 *
 * @author Simon Kallweit, simon@weirdsoft.ch
 */

#ifndef __BACKLIGHT_H__
#define __BACKLIGHT_H__

extern struct backlight_ao backlight_ao;

void backlight_ctor(void);

#endif // __BACKLIGHT_H__
