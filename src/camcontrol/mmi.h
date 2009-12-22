
#ifndef __MMI_H__
#define __MMI_H__

/* Commands */
#define CMD_SINGLE_SHOT		0
#define CMD_SPHERICAL_PAN	1
#define CMD_GIGA_PAN		2
#define CMD_TIMELAPSE		3
#define CMD_SAVE			4
#define CMD_SERVO_MIN		5
#define CMD_SERVO_CENTER	6
#define CMD_SERVO_MAX		7

extern struct mmi_ao mmi_ao;

void mmi_ctor(void);

#endif // __MMI_H__
