
#ifndef __MENU_PARAM_H__
#define __MENU_PARAM_H__

#include <stdio.h>
#include "lcd.h"
#include "key.h"
#include "globals.h"

static int modify_uint(menu_item_t item, uint32_t *value, int dir, int shift)
{
	uint8_t step = shift ? item->u.param->shift_step : item->u.param->step;

	if (dir == ENC_UP && *value > item->u.param->min) {
		if (step > *value - item->u.param->min)
			step = *value - item->u.param->min;
		(*value) -= step;
		return 1;
	}
	if (dir == ENC_DOWN && *value < item->u.param->max) {
		if (step > item->u.param->max - *value)
			step = item->u.param->max - *value;
		(*value) += step;
		return 1;
	}

	return 0;
}

static int modify_uint8(menu_item_t item, int dir, int shift)
{
	int ret;
	uint32_t value = *((uint8_t *) item->u.param->data);
	ret = modify_uint(item, &value, dir, shift);
	if (ret)
		*((uint8_t *) item->u.param->data) = value;
	return ret;
}

static int modify_uint16(menu_item_t item, int dir, int shift)
{
	int ret;
	uint32_t value = *((uint16_t *) item->u.param->data);
	ret = modify_uint(item, &value, dir, shift);
	if (ret)
		*((uint16_t *) item->u.param->data) = value;
	return ret;
}

static int modify_uint32(menu_item_t item, int dir, int shift)
{
	int ret;
	uint32_t value = *((uint32_t *) item->u.param->data);
	ret = modify_uint(item, &value, dir, shift);
	if (ret)
		*((uint32_t *) item->u.param->data) = value;
	return ret;
}

static void print_decimal(menu_item_t item)
{
	char tmp[17];
	uint16_t *value = item->u.param->data;

	snprintf(tmp, sizeof(tmp), "%u", *value);
	lcd_write(0, 1, tmp, LCD_FILL_BLANK);
}

static void print_mm(menu_item_t item)
{
	char tmp[17];
	uint16_t *value = item->u.param->data;

	snprintf(tmp, sizeof(tmp), "%u.%01u mm", *value / 10, *value % 10);
	lcd_write(0, 1, tmp, LCD_FILL_BLANK);
}

static void print_deg(menu_item_t item)
{
	char tmp[17];
	uint16_t *value = item->u.param->data;

	snprintf(tmp, sizeof(tmp), "%u deg", *value);
	lcd_write(0, 1, tmp, LCD_FILL_BLANK);
}

static void print_shutter_time(menu_item_t item)
{
	char tmp[17];
	uint8_t *value = item->u.param->data;

	snprintf(tmp, sizeof(tmp), "%s", shutter_time[*value].name);
	lcd_write(0, 1, tmp, LCD_FILL_BLANK);
}

static void print_shutter_mode(menu_item_t item)
{
	char tmp[17];
	uint8_t *value = item->u.param->data;

	snprintf(tmp, sizeof(tmp), "%s", shutter_mode[*value]);
	lcd_write(0, 1, tmp, LCD_FILL_BLANK);
}

static void print_trigger_delay(menu_item_t item)
{
	char tmp[17];
	uint16_t *value = item->u.param->data;

	snprintf(tmp, sizeof(tmp), "%u.%02u", *value / 100, *value % 100);
	lcd_write(0, 1, tmp, LCD_FILL_BLANK);
}

static void print_timelapse_rate(menu_item_t item)
{
	char tmp[17];
	uint32_t *value = item->u.param->data;

	snprintf(tmp, sizeof(tmp), "%02lu:%02lu:%02lu", *value / 3600, (*value / 60) % 60, *value % 60);
	lcd_write(0, 1, tmp, LCD_FILL_BLANK);
}


MENU_PARAM(param_pan_width, &globals.pan_width, 0, 360, 1, 10, modify_uint16, print_deg, NULL);
MENU_PARAM(param_pan_height, &globals.pan_height, 0, 180, 1, 10, modify_uint16, print_deg, NULL);
MENU_PARAM(param_sensor_width, &globals.sensor_width, 100, 500, 1, 10, modify_uint16, print_mm, NULL);
MENU_PARAM(param_sensor_height, &globals.sensor_height, 100, 500, 1, 10, modify_uint16, print_mm, NULL);
MENU_PARAM(param_focal_length, &globals.focal_length, 100, 500, 1, 10, modify_uint16, print_mm, NULL);
MENU_PARAM(param_shutter_mode, &globals.shutter_mode, 0, NUM_SHUTTER_MODES - 1, 1, 1, modify_uint8, print_shutter_mode, NULL);
MENU_PARAM(param_hdr_time1, &globals.hdr_time1, 0, NUM_SHUTTER_TIMES - 1, 1, 5, modify_uint8, print_shutter_time, NULL);
MENU_PARAM(param_hdr_time2, &globals.hdr_time2, 0, NUM_SHUTTER_TIMES - 1, 1, 5, modify_uint8, print_shutter_time, NULL);
MENU_PARAM(param_hdr_shots, &globals.hdr_shots, 2, 10, 1, 1, modify_uint16, print_decimal, NULL);
MENU_PARAM(param_trigger_delay, &globals.trigger_delay, 10, 500, 1, 10, modify_uint16, print_trigger_delay, NULL);
MENU_PARAM(param_timelapse_rate, &globals.timelapse_rate, 1, 100000, 1, 60, modify_uint32, print_timelapse_rate, NULL);

#endif // __MENU_PARAM_H__
