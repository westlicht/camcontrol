
#ifndef __MENU_PARAM_H__
#define __MENU_PARAM_H__

#include <stdio.h>
#include "lcd.h"
#include "key.h"
#include "globals.h"




static int modify_param(menu_item_t item, int dir)
{
	uint16_t *value = item->u.param->data;

	if (dir == ENC_UP && *value > item->u.param->min) {
		(*value)--;
		return 1;
	}
	if (dir == ENC_DOWN && *value < item->u.param->max) {
		(*value)++;
		return 1;
	}

	return 0;
}

static void print_mm(menu_item_t item)
{
	char tmp[17];
	uint16_t *value = item->u.param->data;

	snprintf(tmp, sizeof(tmp), "%u.%01u mm", *value / 10, *value % 10);
	lcd_write(0, 1, tmp);
}

static void print_deg(menu_item_t item)
{
	char tmp[17];
	uint16_t *value = item->u.param->data;

	snprintf(tmp, sizeof(tmp), "%u deg", *value);
	lcd_write(0, 1, tmp);
}


MENU_PARAM(param_pan_width, &globals.pan_width, 0, 360, modify_param, print_deg, NULL);
MENU_PARAM(param_pan_height, &globals.pan_height, 0, 180, modify_param, print_deg, NULL);
MENU_PARAM(param_sensor_width, &globals.sensor_width, 100, 500, modify_param, print_mm, NULL);
MENU_PARAM(param_sensor_height, &globals.sensor_height, 100, 500, modify_param, print_mm, NULL);
MENU_PARAM(param_focal_length, &globals.focal_length, 100, 500, modify_param, print_mm, NULL);




#endif // __MENU_PARAM_H__
