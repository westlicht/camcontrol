#ifndef __MENU_DEF_H__
#define __MENU_DEF_H__

#include "globals.h"
#include "menu_param.h"

extern void start_panorama_handler(void);
extern void save_settings_handler(void);

static const struct menu_item page_panorama[] = {
	{
		MENU_ITEM_PARAM("Panorama width", &param_pan_width),
	}, {
		MENU_ITEM_PARAM("Panorama height", &param_pan_height),
	}, {
		MENU_ITEM("Focal length"),
	}, {
		MENU_ITEM_CMD("Start", start_panorama_handler),
	}, {
		MENU_ITEM_LAST(),
	}
};

static const struct menu_item page_hdr[] = {
	{
		MENU_ITEM_PARAM("Shutter time 1", &param_hdr_time1),
	}, {
		MENU_ITEM_PARAM("Shutter time 2", &param_hdr_time2),
	}, {
		MENU_ITEM_PARAM("Num of shots", &param_hdr_shots),
	}, {
		MENU_ITEM_LAST(),
	}
};

static const struct menu_item page_setup[] = {
	{
		MENU_ITEM_PARAM("Focal length", &param_focal_length),
	}, {
		MENU_ITEM_PARAM("Sensor width", &param_sensor_width),
	}, {
		MENU_ITEM_PARAM("Sensor height", &param_sensor_height),
	}, {
		MENU_ITEM_CMD("Save", save_settings_handler),
	}, {
		MENU_ITEM_LAST(),
	}
};

static const struct menu_item page_main[] = {
	{
		MENU_ITEM_SUB("Panorama", page_panorama),
	}, {
		MENU_ITEM_SUB("HDR", page_hdr),
	}, {
		MENU_ITEM_SUB("Settings", page_setup),
	}, {
		MENU_ITEM_LAST(),
	}
};

#endif // __MENU_DEF_H__
