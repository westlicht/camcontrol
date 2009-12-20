/**
 * Shutter control using 8-bit timer (timer2).
 */

#ifndef __SHUTTER_H__
#define __SHUTTER_H__

void shutter_init(void);
void shutter_trigger(uint32_t us);

#endif // __SHUTTER_H__
