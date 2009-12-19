#ifndef __GLOBALS_H__
#define __GLOBALS_H__

#include <stdint.h>

/** Global variables structure */
struct globals {
	uint16_t focal_length;		/**< Focal length (mm * 0.1) */
	uint16_t sensor_width;		/**< Sensor width (mm * 0.1) */
	uint16_t sensor_height;		/**< Sensor height (mm * 0.1) */
	uint16_t fov_h;				/**< Horizontal field of view (deg * 0.01) */
	uint16_t fov_v;				/**< Vertical field of view (deg * 0.01) */
	uint16_t center_h;			/**< Horizontal center position (deg * 0.01) */
	uint16_t center_v;			/**< Vertical center position (deg * 0.01) */
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
