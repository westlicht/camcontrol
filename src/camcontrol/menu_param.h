
#ifndef __MENU_PARAM_H__
#define __MENU_PARAM_H__

#include <stdio.h>
#include "lcd.h"
#include "key.h"

#define SENSOR_DIM_MIN		100
#define SENSOR_DIM_MAX		500

#define FOCAL_LENGTH_MIN	100
#define FOCAL_LENGTH_MAX	5000

// Sensor dimension

static int modify_sensor_dim(menu_item_t item, int dir)
{
	uint16_t *value = item->u.param.data;

	if (dir == ENC_UP && *value > SENSOR_DIM_MIN) {
		(*value)--;
		return 1;
	}
	if (dir == ENC_DOWN && *value < SENSOR_DIM_MAX) {
		(*value)++;
		return 1;
	}

	return 0;
}

static void print_sensor_dim(menu_item_t item)
{
	char tmp[17];
	uint16_t *value = item->u.param.data;

	snprintf(tmp, sizeof(tmp), "%u.%01u mm", *value / 10, *value % 10);
	lcd_write(0, 1, tmp);
}

// Focal length

static int modify_focal_length(menu_item_t item, int dir)
{
	uint16_t *value = item->u.param.data;

	if (dir == ENC_UP && *value > FOCAL_LENGTH_MIN) {
		(*value)--;
		return 1;
	}
	if (dir == ENC_DOWN && *value < FOCAL_LENGTH_MAX) {
		(*value)++;
		return 1;
	}

	return 0;
}

static void print_focal_length(menu_item_t item)
{
	char tmp[17];
	uint16_t *value = item->u.param.data;

	snprintf(tmp, sizeof(tmp), "%u.%01u mm", *value / 10, *value % 10);
	lcd_write(0, 1, tmp);
}

#endif // __MENU_PARAM_H__
