#ifndef __MENU_DEF_H__
#define __MENU_DEF_H__

#include "globals.h"

static const struct menu_item page_panorama[] = {
	{
		MENU_ITEM("Shoot"),
	}, {
		MENU_ITEM_LAST(),
	}
};

static const struct menu_item page_hdr[] = {
	{
		MENU_ITEM("Shoot 1"),
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
		MENU_ITEM_LAST(),
	}
};

static const struct menu_item page_main[] = {
	{
		MENU_ITEM_SUB("Panorama", page_panorama),
	}, {
		MENU_ITEM_SUB("HDR", page_hdr),
	}, {
		MENU_ITEM_SUB("Setup", page_setup),
	}, {
		MENU_ITEM_LAST(),
	}
};

#endif // __MENU_DEF_H__
