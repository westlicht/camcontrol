
#ifndef __PARAM_H__
#define __PARAM_H__

#include <stdlib.h>
#include <stdint.h>
#include <avr/eeprom.h>
#include <util/crc16.h>

/** Parameter data structure */
struct param_data {
	uint16_t focal_length;		/**< Focal length (mm * 0.1) */
	uint16_t sensor_width;		/**< Sensor width (mm * 0.1) */
	uint16_t sensor_height;		/**< Sensor height (mm * 0.1) */
	uint16_t fov_h;				/**< Horizontal field of view (deg * 0.01) */
	uint16_t fov_v;				/**< Vertical field of view (deg * 0.01) */
	uint16_t center_h;			/**< Horizontal center position (deg) */
	uint16_t center_v;			/**< Vertical center position (deg) */
	uint16_t pan_width;			/**< Panorama width (deg) */
	uint16_t pan_height;		/**< Panorama height (deg) */
	uint8_t shutter_mode;		/**< Shutter mode */
	uint8_t hdr_time1;			/**< Min exposure time for HDR series */
	uint8_t hdr_time2;			/**< Max exposure time for HDR series */
	uint16_t hdr_shots;			/**< Number of shots per HDR series */
	uint16_t trigger_delay;		/**< Trigger delay between shots in s * 0.01 */
	uint32_t timelapse_rate;	/**< Timelapse trigger rate in s */
};

/** Parameter data */
extern struct param_data pd;

/** Shutter time info */
struct shutter_time {
	const char *name;
	const uint32_t us;
};

/** Shutter time infos */
#define NUM_SHUTTER_TIMES	54
extern struct shutter_time shutter_time[NUM_SHUTTER_TIMES];

/** Shutter modes */
#define NUM_SHUTTER_MODES 	2
extern const char *shutter_mode[NUM_SHUTTER_MODES];

/** Parameter types */
typedef enum {
	UINT8,
	UINT16,
	UINT32,
} param_typ_t;

/* Forward declaration */
struct param;

/** Parameter class definition */
struct param_class {
	param_typ_t typ;
	uint32_t min;
	uint32_t max;
	uint32_t def;
	uint8_t step;
	uint8_t shift_step;
	void (*print)(const struct param *param, char *str, int len);
};

/** Parameter definition */
struct param {
	const struct param_class *class;
	void *value;
};


#define PARAM_CLASS(_name_, _typ_, _min_, _max_, _default_, _step_, _shift_step_, _print_)
#define PARAM(_name_, _class_, _value_) extern const struct param _name_;
#include "param_def.h"
#undef PARAM_CLASS
#undef PARAM

void param_init(void);
void param_defaults(void);
int param_load(void);
void param_save(void);

uint32_t param_get(const struct param *param);
void param_set(const struct param *param, uint32_t value);
void param_print(const struct param *param, char *str, int len);

#endif // __PARAM_H__
