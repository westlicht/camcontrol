
#include <stdint.h>
#include <math.h>

#include <avr/eeprom.h>
#include <util/crc16.h>

#include "globals.h"

/** Shutter time infos */
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
	{ "0\"3", 300000 },
	{ "0\"4", 400000 },
	{ "0\"5", 500000 },
	{ "0\"6", 600000 },
	{ "0\"8", 800000 },
	{ "1\"", 1000000 },
	{ "1\"3", 1300000 },
	{ "1\"6", 1600000 },
	{ "2\"", 2000000 },
	{ "2\"5", 2500000 },
	{ "3\"2", 3200000 },
	{ "4\"", 4000000 },
	{ "5\"", 5000000 },
	{ "6\"", 6000000 },
	{ "8\"", 8000000 },
	{ "10\"", 10000000 },
	{ "13\"", 13000000 },
	{ "15\"", 15000000 },
	{ "20\"", 20000000 },
	{ "25\"", 25000000 },
	{ "30\"", 30000000 },
};

/** Shutter modes */
const char *shutter_mode[] = {
		"Camera", "HDR"
};

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
