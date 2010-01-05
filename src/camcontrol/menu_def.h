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

static const struct menu_item page_camera_setup[] = {
	{
		MENU_ITEM_PARAM("Sensor Width", &p_sensor_width, CMD_NONE),
	}, {
		MENU_ITEM_PARAM("Sensor Height", &p_sensor_height, CMD_NONE),
	}, {
		MENU_ITEM_PARAM("Crop", &p_crop, CMD_NONE),
	}, {
		MENU_ITEM_PARAM("Focal Length", &p_focal_length, CMD_NONE),
	}, {
		MENU_ITEM_LAST(),
	}
};

static const struct menu_item page_shutter_setup[] = {
	{
		MENU_ITEM_PARAM("Mode", &p_shutter_mode, CMD_NONE),
	}, {
		MENU_ITEM_PARAM("Short Exposure", &p_shutter_time, CMD_NONE),
	}, {
		MENU_ITEM_PARAM("Long Exposure", &p_shutter_long_time, CMD_NONE),
	}, {
		MENU_ITEM_PARAM("HDR Min Exposure", &p_hdr_time1, CMD_NONE),
	}, {
		MENU_ITEM_PARAM("HDR Max Exposure", &p_hdr_time2, CMD_NONE),
	}, {
		MENU_ITEM_PARAM("HDR Shots", &p_hdr_shots, CMD_NONE),
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
		MENU_ITEM_CMD("Start", CMD_SINGLE_SHOT),
	}, {
		MENU_ITEM_LAST(),
	}
};

static const struct menu_item page_spherical_pan[] = {
	{
		MENU_ITEM_PARAM("Overlap", &p_spherical_overlap, CMD_NONE),
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
		MENU_ITEM_CMD("Start", CMD_GIGA_PAN),
	}, {
		MENU_ITEM_LAST(),
	}
};

static const struct menu_item page_timelapse[] = {
	{
		MENU_ITEM_PARAM("Trigger rate", &p_timelapse_rate, CMD_NONE),
	}, {
		MENU_ITEM_CMD("Start", CMD_TIMELAPSE),
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
		MENU_ITEM_SUB("Camera Setup", page_camera_setup),
	}, {
		MENU_ITEM_SUB("Shutter Setup", page_shutter_setup),
	}, {
		MENU_ITEM_SUB("Single Shot", page_single_shot),
	}, {
		MENU_ITEM_SUB("Spherical Pan", page_spherical_pan),
	}, {
		MENU_ITEM_SUB("Giga Pan", page_giga_pan),
	}, {
		MENU_ITEM_SUB("Timelapse", page_timelapse),
	}, {
		MENU_ITEM_CMD("Save Settings", CMD_SAVE),
	}, {
		MENU_ITEM_SUB("Test", page_test),
	}, {
		MENU_ITEM_LAST(),
	}
};

#endif // __MENU_DEF_H__
