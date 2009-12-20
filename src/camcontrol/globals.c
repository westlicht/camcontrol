
#include <stdint.h>
#include <math.h>

#include <avr/eeprom.h>
#include <avr/crc16.h>

#include "globals.h"

/** EEPROM magic number */
#define EEPROM_MAGIC	0xfe45

/** EEPROM header */
struct eeprom_header {
	uint16_t magic;		/**< Magic number */
	uint16_t crc16;		/**< CRC16 of data */
};

/** Global variables */
struct globals globals;

/** First byte in EEPROM */
static char eemem EEMEM;

/**
 * Computes the field of view angle based on the focal length and the image size.
 * The field of view is computed by:
 *   a = 2 * arctan(d/2f)
 *   a = Field of view
 *   d = Image size
 *   f = Focal length
 * @param focal_length Focal length
 * @param size Image size
 * @return Returns the field of view angle (deg * 0.01)
 */
static uint16_t compute_fov_angle(uint16_t focal_length, uint16_t size)
{
	float f = focal_length;
	float d = size;
	float a;

	if (f == 0.0)
		return 0;

	a = atan(d / 2.0 * f) * 2.0;

	return floor(a * 100.0);
}

/**
 * Computes the CRC16 of the globals data block.
 * @return Returns the CRC16.
 */
static uint16_t compute_crc16(void)
{
	uint16_t crc16 = 0;
	int i;

	for (i = 0; i < sizeof(globals); i++)
		crc16 = _crc16_update(crc16, ((uint8_t *) &globals)[i]);

	return crc16;
}


/**
 * Loads default values.
 */
void globals_default(void)
{
	globals.focal_length = 200;
	globals.sensor_width = 100;
	globals.sensor_height = 100;
	globals.fov_h = compute_fov_angle(globals.focal_length, globals.sensor_width);
	globals.fov_v = compute_fov_angle(globals.focal_length, globals.sensor_height);
	globals.center_h = 0;
	globals.center_v = 0;
	globals.hdr_time1 = 0;
	globals.hdr_time2 = 0;
	globals.hdr_shots = 5;
}

/**
 * Loads the globals from EEPROM.
 * @return Returns 0 if successful.
 */
int globals_load(void)
{
	struct eeprom_header header;
	void *addr = &eemem;

	eeprom_read_block(&header, addr, sizeof(header));

	if (header.magic != EEPROM_MAGIC)
		return -1;

	addr += sizeof(header);
	eeprom_read_block(&globals, addr, sizeof(globals));

	if (header.crc16 != compute_crc16())
		return -1;

	return 0;
}

/**
 * Saves the globals to EEPROM.
 */
void globals_save(void)
{
	struct eeprom_header header;
	void *addr = &eemem;

	header.magic = EEPROM_MAGIC;
	header.crc16 = compute_crc16();

	eeprom_write_block(&header, addr, sizeof(header));
	addr += sizeof(header);
	eeprom_write_block(&globals, addr, sizeof(globals));
}

/**
 * Sets the focal length and recomputes the field of view angles.
 */
void globals_set_focal_length(uint16_t focal_length)
{
	globals.focal_length = focal_length;
	globals.fov_h = compute_fov_angle(globals.focal_length, globals.sensor_width);
	globals.fov_v = compute_fov_angle(globals.focal_length, globals.sensor_height);
}

/**
 * Sets the sensor width and recomputes the field of view angle.
 */
void globals_set_sensor_width(uint16_t sensor_width)
{
	globals.sensor_width = sensor_width;
	globals.fov_h = compute_fov_angle(globals.focal_length, globals.sensor_width);
}

/**
 * Sets the sensor height and recomputes the field of view angle.
 */
void globals_set_sensor_height(uint16_t sensor_height)
{
	globals.sensor_height = sensor_height;
	globals.fov_v = compute_fov_angle(globals.focal_length, globals.sensor_height);
}
