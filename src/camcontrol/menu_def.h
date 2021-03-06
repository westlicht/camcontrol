/** @file menu_def.h
 *
 * Menu definition file.
 *
 * @author Simon Kallweit, simon@weirdsoft.ch
 */

#ifndef __MENU_DEF_H__
#define __MENU_DEF_H__

#include "param.h"
#include "mmi.h"


static const struct menu_item page_exposure_setup[] = {
    {
        MENU_ITEM_PARAM("Mode", &p_shutter_mode, CMD_NONE),
    }, {
        MENU_ITEM_PARAM("Short Exposure", &p_exposure_short, CMD_NONE),
    }, {
        MENU_ITEM_PARAM("Long Exposure", &p_exposure_long, CMD_NONE),
    }, {
        MENU_ITEM_PARAM("Exposure Step", &p_exposure_step, CMD_NONE),
    }, {
        MENU_ITEM_PARAM("HDR Steps [-]", &p_hdr_steps_n, CMD_NONE),
    }, {
        MENU_ITEM_PARAM("HDR Steps [+]", &p_hdr_steps_p, CMD_NONE),
    }, {
        MENU_ITEM_PARAM("Post Delay", &p_trigger_delay, CMD_NONE),
    }, {
        MENU_ITEM_LAST(),
    }
};

static const struct menu_item page_single_shot[] = {
    {
        MENU_ITEM_PARAM("Center X", &p_center_x, CMD_UPDATE_CENTER),
    }, {
        MENU_ITEM_PARAM("Center Y", &p_center_y, CMD_UPDATE_CENTER),
    }, {
        MENU_ITEM_SUB("Exposure Setup", page_exposure_setup),
    }, {
        MENU_ITEM_CMD("Start", CMD_SINGLE_SHOT),
    }, {
        MENU_ITEM_LAST(),
    }
};

static const struct menu_item page_spherical_pan[] = {
    {
        MENU_ITEM_PARAM("Overlap", &p_spherical_overlap, CMD_NONE),
    }, {
        MENU_ITEM_SUB("Exposure Setup", page_exposure_setup),
    }, {
        MENU_ITEM_CMD("Start", CMD_SPHERICAL_PAN),
    }, {
        MENU_ITEM_LAST(),
    }
};

static const struct menu_item page_giga_pan[] = {
    {
        MENU_ITEM_PARAM("Start X", &p_giga_start_x, CMD_UPDATE_GIGA_START),
    }, {
        MENU_ITEM_PARAM("Start Y", &p_giga_start_y, CMD_UPDATE_GIGA_START),
    }, {
        MENU_ITEM_PARAM("End X", &p_giga_end_x, CMD_UPDATE_GIGA_END),
    }, {
        MENU_ITEM_PARAM("End Y", &p_giga_end_y, CMD_UPDATE_GIGA_END),
    }, {
        MENU_ITEM_PARAM("Overlap", &p_giga_overlap, CMD_NONE),
    }, {
        MENU_ITEM_SUB("Exposure Setup", page_exposure_setup),
    }, {
        MENU_ITEM_CMD("Start", CMD_GIGA_PAN),
    }, {
        MENU_ITEM_LAST(),
    }
};

static const struct menu_item page_timelapse[] = {
    {
        MENU_ITEM_PARAM("Trigger rate", &p_timelapse_rate, CMD_NONE),
    }, {
        MENU_ITEM_SUB("Exposure Setup", page_exposure_setup),
    }, {
        MENU_ITEM_CMD("Start", CMD_TIMELAPSE),
    }, {
        MENU_ITEM_LAST(),
    }
};




static const struct menu_item page_servo_setup[] = {
    {
        MENU_ITEM_PARAM("Horizontal Min", &p_servo_min_x, CMD_UPDATE_SERVO_MIN_X),
    }, {
        MENU_ITEM_PARAM("Horizontal Max", &p_servo_max_x, CMD_UPDATE_SERVO_MAX_X),
    }, {
        MENU_ITEM_PARAM("Vertical Min", &p_servo_min_y, CMD_UPDATE_SERVO_MIN_Y),
    }, {
        MENU_ITEM_PARAM("Vertical Max", &p_servo_max_y, CMD_UPDATE_SERVO_MAX_Y),
    }, {
        MENU_ITEM_LAST(),
    }
};

static const struct menu_item page_setup[] = {
    {
        MENU_ITEM_PARAM("Sensor Width", &p_sensor_width, CMD_NONE),
    }, {
        MENU_ITEM_PARAM("Sensor Height", &p_sensor_height, CMD_NONE),
    }, {
        MENU_ITEM_PARAM("Crop", &p_crop, CMD_NONE),
    }, {
        MENU_ITEM_PARAM("Focal Length", &p_focal_length, CMD_NONE),
    }, {
        MENU_ITEM_PARAM("Backlight Time", &p_misc_backlight, CMD_NONE),
    }, {
        MENU_ITEM_SUB("Servo Calibration", page_servo_setup),
    }, {
        MENU_ITEM_CMD("Save Settings", CMD_SAVE),
    }, {
        MENU_ITEM_LAST(),
    }
};

static const struct menu_item page_test[] = {
    {
        MENU_ITEM_CMD("Servo min", CMD_SERVO_MIN),
    }, {
        MENU_ITEM_CMD("Servo center", CMD_SERVO_CENTER),
    }, {
        MENU_ITEM_CMD("Servo max", CMD_SERVO_MAX),
    }, {
        MENU_ITEM_LAST(),
    }
};

static const struct menu_item page_main[] = {
    {
        MENU_ITEM_SUB("Single Shot", page_single_shot),
    }, {
        MENU_ITEM_SUB("Spherical Pan", page_spherical_pan),
    }, {
        MENU_ITEM_SUB("Giga Pan", page_giga_pan),
    }, {
        MENU_ITEM_SUB("Timelapse", page_timelapse),
    }, {
        MENU_ITEM_SUB("Setup", page_setup),
    }, {
        MENU_ITEM_CMD("Shutdown", CMD_SHUTDOWN),
    }, {
        MENU_ITEM_SUB("Test", page_test),
    }, {
        MENU_ITEM_LAST(),
    }
};

#endif // __MENU_DEF_H__
