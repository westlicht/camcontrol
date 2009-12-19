#ifndef __MENU_DEF_H__
#define __MENU_DEF_H__

#include "globals.h"

extern void start_panorama_handler(void);
extern void save_settings_handler(void);

static const struct menu_item page_panorama[] = {
	{
		MENU_ITEM("Panorama width"),
	}, {
		MENU_ITEM("Panorama height"),
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
		MENU_ITEM_CMD("Shoot 1", NULL),
	}, {
		MENU_ITEM_LAST(),
	}
};

static const struct menu_item page_setup[] = {
	{
		MENU_ITEM("Focal length"),
	}, {
		MENU_ITEM("Sensor width"),
	}, {
		MENU_ITEM("Sensor height"),
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
