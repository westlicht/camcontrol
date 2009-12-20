#ifndef __GLOBALS_H__
#define __GLOBALS_H__

#include <stdint.h>

/** Shutter time info */
struct shutter_time {
	const char *name;
	const uint32_t us;
};

/** Shutter time infos */
extern struct shutter_time shutter_time[54];
#define NUM_SHUTTER_TIMES (sizeof(shutter_time) / (sizeof(shutter_time[0])))

/** Shutter modes */
extern const char *shutter_mode[2];
#define NUM_SHUTTER_MODES (sizeof(shutter_mode) / sizeof(shutter_mode[0]))


/** Global variables structure */
struct globals {
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

/** Global variables */
extern struct globals globals;

void globals_default(void);
int globals_load(void);
void globals_save(void);

void globals_set_focal_length(uint16_t focal_length);
void globals_set_sensor_width(uint16_t sensor_width);
void globals_set_sensor_height(uint16_t sensor_height);

#endif // __GLOBALS_H__
