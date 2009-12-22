#ifndef __MENU_DEF_H__
#define __MENU_DEF_H__

#include "param.h"

extern void exec_single_shot(void);
extern void exec_spherical_pan(void);
extern void exec_giga_pan(void);
extern void exec_timelapse(void);
extern void exec_save(void);
extern void exec_servo_min(void);
extern void exec_servo_center(void);
extern void exec_servo_max(void);

static const struct menu_item page_single_shot[] = {
	{
		MENU_ITEM_PARAM("Shutter mode", &p_shutter_mode),
	}, {
		MENU_ITEM_CMD("Start", exec_single_shot),
	}, {
		MENU_ITEM_LAST(),
	}
};

static const struct menu_item page_spherical_pan[] = {
	{
		MENU_ITEM_PARAM("Focal length", &p_focal_length),
	}, {
		MENU_ITEM_PARAM("Shutter mode", &p_shutter_mode),
	}, {
		MENU_ITEM_CMD("Start", exec_spherical_pan),
	}, {
		MENU_ITEM_LAST(),
	}
};

static const struct menu_item page_giga_pan[] = {
	{
		MENU_ITEM_PARAM("Focal length", &p_focal_length),
	}, {
		MENU_ITEM_PARAM("Shutter mode", &p_shutter_mode),
	}, {
		MENU_ITEM_CMD("Start", exec_giga_pan),
	}, {
		MENU_ITEM_LAST(),
	}
};

static const struct menu_item page_timelapse[] = {
	{
		MENU_ITEM_PARAM("Trigger rate", &p_timelapse_rate),
	}, {
		MENU_ITEM_CMD("Start", exec_timelapse),
	}, {
		MENU_ITEM_LAST(),
	}
};

static const struct menu_item page_setup[] = {
	{
		MENU_ITEM_PARAM("Focal length", &p_focal_length),
	}, {
		MENU_ITEM_PARAM("Sensor width", &p_sensor_width),
	}, {
		MENU_ITEM_PARAM("Sensor height", &p_sensor_height),
	}, {
		MENU_ITEM_PARAM("HDR min exposure", &p_hdr_time1),
	}, {
		MENU_ITEM_PARAM("HDR max exposure", &p_hdr_time2),
	}, {
		MENU_ITEM_PARAM("HDR shots", &p_hdr_shots),
	}, {
		MENU_ITEM_PARAM("Trigger delay", &p_trigger_delay),
	}, {
		MENU_ITEM_CMD("Save", exec_save),
	}, {
		MENU_ITEM_LAST(),
	}
};

static const struct menu_item page_test[] = {
	{
		MENU_ITEM_CMD("Servo min", exec_servo_min),
	}, {
		MENU_ITEM_CMD("Servo center", exec_servo_center),
	}, {
		MENU_ITEM_CMD("Servo max", exec_servo_max),
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
