/** @file mmi.h
 *
 * MMI active object. Handles machine man interface.
 *
 * @author Simon Kallweit, simon@weirdsoft.ch
 */

#ifndef __MMI_H__
#define __MMI_H__

/* Commands */
enum commands {
    CMD_NONE,
    /* Regular commands */
    CMD_SINGLE_SHOT,
    CMD_SPHERICAL_PAN,
    CMD_GIGA_PAN,
    CMD_TIMELAPSE,
    CMD_SAVE,
    CMD_SERVO_MIN,
    CMD_SERVO_CENTER,
    CMD_SERVO_MAX,
    /* Servo setup commands */
    CMD_UPDATE_SERVO_MIN_X,
    CMD_UPDATE_SERVO_MAX_X,
    CMD_UPDATE_SERVO_MIN_Y,
    CMD_UPDATE_SERVO_MAX_Y,
    /* Parameter commands */
    CMD_UPDATE_CENTER,
    CMD_UPDATE_GIGA_START,
    CMD_UPDATE_GIGA_END,
    /* Misc commands */
    CMD_SHUTDOWN,
};

extern struct mmi_ao mmi_ao;

void mmi_ctor(void);

#endif // __MMI_H__
