/** @file param.h
 *
 * Parameter handling. Parameter are variables which can be edited via the MMI
 * and are presistently stored in EEPROM.
 *
 * @author Simon Kallweit, simon@weirdsoft.ch
 */

#ifndef __PARAM_H__
#define __PARAM_H__

#include <stdlib.h>
#include <stdint.h>
#include <avr/eeprom.h>
#include <util/crc16.h>

/** Parameter data structure */
struct param_data {
	/* Camera parameters */
	struct {
		uint16_t focal_length;	/**< Focal length (mm * 0.1) */
		uint16_t sensor_width;	/**< Sensor width (mm * 0.1) */
		uint16_t sensor_height;	/**< Sensor height (mm * 0.1) */
		uint16_t crop;			/**< Crop factor in 0.01 */
	} camera;
	/* Shutter parameters */
	struct {
		uint8_t mode;			/**< Shutter mode */
		uint8_t time;			/**< Exposure time */
		uint32_t long_time;		/**< Long exposure time */
		uint8_t hdr_time1;		/**< Min exposure time for HDR series */
		uint8_t hdr_time2;		/**< Max exposure time for HDR series */
		uint16_t hdr_shots;		/**< Number of shots per HDR series */
		uint16_t post_delay;	/**< Post delay between shots in s * 0.01 */
	} shutter;
	/* Single shot parameters */
	struct {
		uint16_t center_x;			/**< Horizontal center position (deg) */
		uint16_t center_y;			/**< Vertical center position (deg) */
	} single;
	/* Spherical pan parameters */
	struct {
		uint16_t overlap;		/**< Minimal image overlap (deg * 0.01) */
	} spherical;
	/* Giga pan parameters */
	struct {
		uint16_t start_x;		/**< Horizontal start position (deg) */
		uint16_t start_y;		/**< Vertical start position (deg) */
		uint16_t end_x;			/**< Horizontal end position (deg) */
		uint16_t end_y;			/**< Vertical end position (deg) */
		uint16_t overlap;		/**< Minimal image overlap (deg * 0.01) */
	} giga;
	/* Timelapse parameters */
	struct {
		uint32_t rate;			/**< Timelapse trigger rate in s */
	} timelapse;
};

/** Parameter data */
extern struct param_data pd;

/** Shutter time info */
struct exposure_info {
	const char *name;
	const uint32_t us;
};

/** Exposure infos */
#define NUM_EXPOSURE_INFOS	54
extern struct exposure_info exposure_info[NUM_EXPOSURE_INFOS];

/** Shutter modes */
#define NUM_SHUTTER_MODES 	4
extern const char *shutter_mode[NUM_SHUTTER_MODES];

enum shutter_mode {
	SHUTTER_MODE_CAMERA,
	SHUTTER_MODE_SHORT,
	SHUTTER_MODE_LONG,
	SHUTTER_MODE_HDR,
};

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
	uint8_t step;
	uint8_t shift_step;
	void (*print)(const struct param *param, char *str, int len);
};

/** Parameter definition */
struct param {
	const struct param_class *class;
	void *value;
	uint32_t def;
};


#define PARAM_CLASS(_name_, _typ_, _min_, _max_, _step_, _shift_step_, _print_)
#define PARAM(_name_, _class_, _value_, _default_) extern const struct param _name_;
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
