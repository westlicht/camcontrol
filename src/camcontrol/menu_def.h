#ifndef __MENU_DEF_H__
#define __MENU_DEF_H__

#include "param.h"
#include "mmi.h"

static const struct menu_item page_single_shot[] = {
	{
		MENU_ITEM_PARAM("Center X", &p_center_x, CMD_UPDATE_CENTER),
	}, {
		MENU_ITEM_PARAM("Center Y", &p_center_y, CMD_UPDATE_CENTER),
	}, {
		MENU_ITEM_PARAM("Shutter mode", &p_shutter_mode, CMD_NONE),
	}, {
		MENU_ITEM_CMD("Start", CMD_SINGLE_SHOT),
	}, {
		MENU_ITEM_LAST(),
	}
};

static const struct menu_item page_spherical_pan[] = {
	{
		MENU_ITEM_PARAM("Focal length", &p_focal_length, CMD_NONE),
	}, {
		MENU_ITEM_PARAM("Shutter mode", &p_shutter_mode, CMD_NONE),
	}, {
		MENU_ITEM_CMD("Start", CMD_SPHERICAL_PAN),
	}, {
		MENU_ITEM_LAST(),
	}
};

static const struct menu_item page_giga_pan[] = {
	{
		MENU_ITEM_PARAM("Focal length", &p_focal_length, CMD_NONE),
	}, {
		MENU_ITEM_PARAM("Shutter mode", &p_shutter_mode, CMD_NONE),
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

static const struct menu_item page_setup[] = {
	{
		MENU_ITEM_PARAM("Focal length", &p_focal_length, CMD_NONE),
	}, {
		MENU_ITEM_PARAM("Sensor width", &p_sensor_width, CMD_NONE),
	}, {
		MENU_ITEM_PARAM("Sensor height", &p_sensor_height, CMD_NONE),
	}, {
		MENU_ITEM_PARAM("HDR min exposure", &p_hdr_time1, CMD_NONE),
	}, {
		MENU_ITEM_PARAM("HDR max exposure", &p_hdr_time2, CMD_NONE),
	}, {
		MENU_ITEM_PARAM("HDR shots", &p_hdr_shots, CMD_NONE),
	}, {
		MENU_ITEM_PARAM("Trigger delay", &p_trigger_delay, CMD_NONE),
	}, {
		MENU_ITEM_CMD("Save", CMD_SAVE),
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
		MENU_ITEM_SUB("Single shot", page_single_shot),
	}, {
		MENU_ITEM_SUB("Spherical pan", page_spherical_pan),
	}, {
		MENU_ITEM_SUB("Giga pan", page_giga_pan),
	}, {
		MENU_ITEM_SUB("Timelapse", page_timelapse),
	}, {
		MENU_ITEM_SUB("Settings", page_setup),
	}, {
		MENU_ITEM_SUB("Test", page_test),
	}, {
		MENU_ITEM_LAST(),
	}
};

#endif // __MENU_DEF_H__
