/** @file param.c
 *
 * Parameter handling. Parameter are variables which can be edited via the MMI
 * and are presistently stored in EEPROM.
 *
 * @author Simon Kallweit, simon@weirdsoft.ch
 */

#include <stdint.h>
#include <stdio.h>
#include "debug.h"
#include "param.h"

/** Parameter data */
struct param_data pd;

/** Shutter time infos */
struct exposure_info exposure_info[NUM_EXPOSURE_INFOS] = {
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
const char *shutter_mode[NUM_SHUTTER_MODES] = {
    "Camera",
    "Short",
    "Long",
    "HDR",
};



static void print_decimal(const struct param *param, char *str, int len)
{
    uint16_t *value = param->value;
    snprintf(str, len, "%u", *value);
}

static void print_mm(const struct param *param, char *str, int len)
{
    uint16_t *value = param->value;
    snprintf(str, len, "%u.%01u mm", *value / 10, *value % 10);
}

static void print_crop(const struct param *param, char *str, int len)
{
    uint16_t *value = param->value;
    snprintf(str, len, "%u.%02u", *value / 100, *value % 100);
}

static void print_deg(const struct param *param, char *str, int len)
{
    uint16_t *value = param->value;
    snprintf(str, len, "%u deg", *value);
}

static void print_exposure(const struct param *param, char *str, int len)
{
    uint8_t *value = param->value;
    snprintf(str, len, "%s", exposure_info[*value].name);
}

static void print_shutter_mode(const struct param *param, char *str, int len)
{
    uint8_t *value = param->value;
    snprintf(str, len, "%s", shutter_mode[*value]);
}

static void print_trigger_delay(const struct param *param, char *str, int len)
{
    uint16_t *value = param->value;
    snprintf(str, len, "%u.%02u", *value / 100, *value % 100);
}

static void print_duration(const struct param *param, char *str, int len)
{
    uint32_t *value = param->value;
    snprintf(str, len, "%02lu:%02lu:%02lu", *value / 3600, (*value / 60) % 60, *value % 60);
}

static void print_overlap(const struct param *param, char *str, int len)
{
    uint16_t *value = param->value;
    snprintf(str, len, "%u.%02u deg", *value / 100, *value % 100);
}




// Parameter class and parameter definitions

#define PARAM_CLASS(_name_, _typ_, _min_, _max_, _step_, _shift_step_, _print_) \
const struct param_class _name_ = {                     \
    .typ = _typ_,                                       \
    .min = _min_,                                       \
    .max = _max_,                                       \
    .step = _step_,                                     \
    .shift_step = _shift_step_,                         \
    .print = _print_,                                   \
};

#define PARAM(_name_, _class_, _value_, _default_)      \
const struct param _name_ = {                           \
    .class = _class_,                                   \
    .value = _value_,                                   \
    .def = _default_,                                   \
};

#include "param_def.h"

#undef PARAM_CLASS
#undef PARAM

// Parameter list definition

#define PARAM_CLASS(_name_, _typ_, _min_, _max_, _step_, _shift_step_, _print_)
#define PARAM(_name_, _class_, _value_, _default_) &_name_,

static const struct param *params[] = {
#include "param_def.h"
};

#undef PARAM_CLASS
#undef PARAM

#define NUM_PARAMS (sizeof(params) / sizeof(params[0]))

// Parameter EEPROM handling

/** EEPROM magic number */
#define EEPROM_MAGIC    0xfe45

/** EEPROM header */
struct eeprom_header {
    uint16_t magic;     /**< Magic number */
    uint16_t crc16;     /**< CRC16 of data */
};

/** First byte in EEPROM */
static char eemem EEMEM;

/**
 * Computes the CRC16 of the parameter data block.
 * @return Returns the CRC16.
 */
static uint16_t compute_crc16(void)
{
    uint16_t crc16 = 0;
    int i;

    for (i = 0; i < sizeof(pd); i++)
        crc16 = _crc16_update(crc16, ((uint8_t *) &pd)[i]);

    return crc16;
}

/**
 * Initializes the parameters. Loads EEPROM or defaults if EEPROM is invalid.
 */
void param_init(void)
{
    if (param_load() != 0)
        param_defaults();
}

/**
 * Loads default parameters.
 */
void param_defaults(void)
{
    int i;
    const struct param *param;

    for (i = 0; i < NUM_PARAMS; i++) {
        param = params[i];
        param_set(param, param->def);
    }
}

/**
 * Loads the parameters from EEPROM.
 * @return Returns 0 if successful.
 */
int param_load(void)
{
    struct eeprom_header header;
    void *addr = &eemem;

    eeprom_read_block(&header, addr, sizeof(header));

    if (header.magic != EEPROM_MAGIC)
        return -1;

    addr += sizeof(header);
    eeprom_read_block(&pd, addr, sizeof(pd));

    if (header.crc16 != compute_crc16())
        return -1;

    return 0;
}

/**
 * Saves the parameters to EEPROM.
 */
void param_save(void)
{
    struct eeprom_header header;
    void *addr = &eemem;

    header.magic = EEPROM_MAGIC;
    header.crc16 = compute_crc16();

    eeprom_write_block(&header, addr, sizeof(header));
    addr += sizeof(header);
    eeprom_write_block(&pd, addr, sizeof(pd));
}

/**
 * Gets a parameters value as an uint32.
 * @param param Parameter
 * @return Returns the value.
 */
uint32_t param_get(const struct param *param)
{
    switch (param->class->typ) {
    case UINT8:
        return *((uint8_t *) param->value);
    case UINT16:
        return *((uint16_t *) param->value);
    case UINT32:
        return *((uint32_t *) param->value);
    }
    ASSERT(0);
    return 0;
}

/**
 * Sets a parameters value from an uint32.
 * @param param Parameter
 * @param value Value to set
 */
void param_set(const struct param *param, uint32_t value)
{
    switch (param->class->typ) {
    case UINT8:
        *((uint8_t *) param->value) = value;
        break;
    case UINT16:
        *((uint16_t *) param->value) = value;
        break;
    case UINT32:
        *((uint32_t *) param->value) = value;
        break;
    default:
        ASSERT(0);
        break;
    }
}

/**
 * Prints the parameter to a string.
 * @param param Parameter
 */
void param_print(const struct param *param, char *str, int len)
{
    param->class->print(param, str, len);
}
