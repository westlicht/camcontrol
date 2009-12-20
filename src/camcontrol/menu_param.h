
#ifndef __MENU_PARAM_H__
#define __MENU_PARAM_H__

#include <stdio.h>
#include "lcd.h"
#include "key.h"
#include "globals.h"


struct shutter_time {
	const char *name;
	const uint32_t us;
};

struct shutter_time shutter_time[] = {
		{ "1/8000", 125 },
		{ "1/6400", 156 },
		{ "1/5000", 200 },
		{ "1/4000", 250 },
		{ "1/3200", 312 },
		{ "1/2500", 400 },
		{ "1/2000", 500 },
		{ "1/1600", 625 },
		{ "1/1250", 800 },
		{ "1/1000", 1000 },
		{ "1/800", 1250 },
		{ "1/640", 1562 },
		{ "1/500", 2000 },
		{ "1/400", 2500 },
		{ "1/320", 3125 },
		{ "1/250", 4000 },
		{ "1/200", 5000 },
		{ "1/160", 6250 },
		{ "1/125", 8000 },
		{ "1/100", 10000 },
		{ "1/80", 12500 },
		{ "1/60", 16667 },
		{ "1/50", 20000 },
		{ "1/40", 25000 },
		{ "1/30", 33333 },
		{ "1/25", 40000 },
		{ "1/20", 50000 },
		{ "1/15", 66667 },
		{ "1/13", 76923 },
		{ "1/10", 100000 },
		{ "1/8", 125000 },
		{ "1/6", 166667 },
		{ "1/4", 250000 },
		{ "0''3", 300000 },
		{ "0''4", 400000 },
		{ "0''5", 500000 },
		{ "0''6", 600000 },
		{ "0''8", 800000 },
		{ "1''", 1000000 },
		{ "1''3", 1300000 },
		{ "1''6", 1600000 },
		{ "2''", 2000000 },
		{ "2''5", 2500000 },
		{ "3''2", 3200000 },
		{ "4''", 4000000 },
		{ "5''", 5000000 },
		{ "6''", 6000000 },
		{ "8''", 8000000 },
		{ "10''", 10000000 },
		{ "13''", 13000000 },
		{ "15''", 15000000 },
		{ "20''", 20000000 },
		{ "25''", 25000000 },
		{ "30''", 30000000 },
};

#define NUM_SHUTTER_TIMES (sizeof(shutter_time) / (sizeof(shutter_time[0])))

const char *shutter_mode[] = {
		"Camera", "HDR"
};

#define NUM_SHUTTER_MODES (sizeof(shutter_mode) / sizeof(shutter_mode[0]))


static int modify_param(menu_item_t item, int dir, int shift)
{
	uint16_t *value = item->u.param->data;
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
	uint16_t *value = item->u.param->data;

	snprintf(tmp, sizeof(tmp), "%s", shutter_time[*value].name);
	lcd_write(0, 1, tmp, LCD_FILL_BLANK);
}

static void print_shutter_mode(menu_item_t item)
{
	char tmp[17];
	uint16_t *value = item->u.param->data;

	snprintf(tmp, sizeof(tmp), "%s", shutter_mode[*value]);
	lcd_write(0, 1, tmp, LCD_FILL_BLANK);
}


MENU_PARAM(param_pan_width, &globals.pan_width, 0, 360, 1, 10, modify_param, print_deg, NULL);
MENU_PARAM(param_pan_height, &globals.pan_height, 0, 180, 1, 10, modify_param, print_deg, NULL);
MENU_PARAM(param_sensor_width, &globals.sensor_width, 100, 500, 1, 10, modify_param, print_mm, NULL);
MENU_PARAM(param_sensor_height, &globals.sensor_height, 100, 500, 1, 10, modify_param, print_mm, NULL);
MENU_PARAM(param_focal_length, &globals.focal_length, 100, 500, 1, 10, modify_param, print_mm, NULL);
MENU_PARAM(param_shutter_mode, &globals.shutter_mode, 0, NUM_SHUTTER_MODES - 1, 1, 1, modify_param, print_shutter_mode, NULL);
MENU_PARAM(param_hdr_time1, &globals.hdr_time1, 0, NUM_SHUTTER_TIMES - 1, 1, 5, modify_param, print_shutter_time, NULL);
MENU_PARAM(param_hdr_time2, &globals.hdr_time2, 0, NUM_SHUTTER_TIMES - 1, 1, 5, modify_param, print_shutter_time, NULL);
MENU_PARAM(param_hdr_shots, &globals.hdr_shots, 2, 10, 1, 1, modify_param, print_decimal, NULL);





#endif // __MENU_PARAM_H__
