/** @file prog.c
 *
 * Program active object. Handles the overall behaviour of the different
 * camera control programs like, spherical pan, giga pan, timelapse etc.
 *
 * @author Simon Kallweit, simon@weirdsoft.ch
 */

#include <math.h>
#include "qpn_port.h"
#include "bsp.h"
#include "camcontrol.h"
#include "debug.h"
#include "shutter.h"
#include "prog.h"
#include "param.h"

#define PI 3.1415 // TODO better precision, math define?

/** Program active object structure */
struct prog_ao {
	QActive super;
	uint32_t seconds;
};

static QState prog_initial(struct prog_ao *me);
static QState prog_idle(struct prog_ao *me);
static QState prog_spherical_pan(struct prog_ao *me);
static QState prog_giga_pan(struct prog_ao *me);
static QState prog_timelapse(struct prog_ao *me);
static QState   prog_timelapse_wait(struct prog_ao *me);

static float compute_fov(float focal_length, float size);

/** Program active object */
struct prog_ao prog_ao;

struct spherical_pan spherical_pan;
struct giga_pan giga_pan;

enum timeouts {
	TIMEOUT_SECOND = TICKS(1000),
};

/**
 * Constructor.
 */
void prog_ctor(void)
{
	QActive_ctor((QActive *) &prog_ao, (QStateHandler) prog_initial);
}

/**
 * Prepares the spherical pan parameters.
 */
void prog_init_spherical_pan(void)
{
	//spherical_pan.fov_x = 10.0;
	//spherical_pan.fov_y = 15.0;
	spherical_pan.tiles_x = 10;
	spherical_pan.tiles_y = 5;
}

/**
 * Prepares the giga pan parameters.
 */
void prog_init_giga_pan(void)
{
	struct giga_pan *info = &giga_pan;
	vec2f_t fov;
	vec2f_t size;
	float overlap;

	// Compute field of view
	fov.x = compute_fov(pd.focal_length / 10.0, pd.sensor_width / 10.0);
	fov.y = compute_fov(pd.focal_length / 10.0, pd.sensor_height / 10.0);
	overlap = pd.giga.overlap / 100.0;

	// Compute size of panorama
	size.x = (pd.giga.end_x - pd.giga.start_x);
	size.y = (pd.giga.end_y - pd.giga.end_x);

	// Compute number of tiles
	if (size.x <= fov.x) {
		info->tiles.x = 1;
	} else {
		info->tiles.x = floor(fabs(size.x / fov.x));
	}
	if (size.y <= fov.y) {
		info->tiles.y = 1;
	} else {
		info->tiles.y = floor(fabs(size.y / fov.y));
	}

	info->pos.x = pd.giga.start_x;
	info->pos.y = pd.giga.start_y;





}

/**
 * Initial state.
 */
static QState prog_initial(struct prog_ao *me)
{
	return Q_TRAN(prog_idle);
}

/**
 * Idle state.
 */
static QState prog_idle(struct prog_ao *me)
{
	switch (Q_SIG(me)) {
	case Q_ENTRY_SIG:
		return Q_HANDLED();
	case Q_EXIT_SIG:
		return Q_HANDLED();
	case Q_TIMEOUT_SIG:
		return Q_HANDLED();
	case SIG_PROG_START:
		switch (Q_PAR(me)) {
		case PROG_SPHERICAL_PAN:
			return Q_TRAN(prog_spherical_pan);
		case PROG_GIGA_PAN:
			return Q_TRAN(prog_giga_pan);
		case PROG_TIMELAPSE:
			return Q_TRAN(prog_timelapse);
		}
		return Q_HANDLED();
	}

	return Q_SUPER(&QHsm_top);
}

/**
 * Spherical panorama state.
 */
static QState prog_spherical_pan(struct prog_ao *me)
{
	switch (Q_SIG(me)) {
	case Q_ENTRY_SIG:
		return Q_HANDLED();
	case Q_EXIT_SIG:
		return Q_HANDLED();
	case Q_TIMEOUT_SIG:
		return Q_HANDLED();
	}

	return Q_SUPER(&QHsm_top);
}

/**
 * Giga panorama state.
 */
static QState prog_giga_pan(struct prog_ao *me)
{
	switch (Q_SIG(me)) {
	case Q_ENTRY_SIG:
		return Q_HANDLED();
	case Q_EXIT_SIG:
		return Q_HANDLED();
	case Q_TIMEOUT_SIG:
		return Q_HANDLED();
	}

	return Q_SUPER(&QHsm_top);
}

/**
 * Timelapse state.
 */
static QState prog_timelapse(struct prog_ao *me)
{
	switch (Q_SIG(me)) {
	case Q_ENTRY_SIG:
		me->seconds = 0;
		QActive_post((QActive *) &shutter_ao, SIG_SHUTTER_START, 0);
		QActive_arm((QActive *) me, TIMEOUT_SECOND);
		return Q_HANDLED();
	case Q_EXIT_SIG:
		QActive_post((QActive *) &shutter_ao, SIG_SHUTTER_STOP, 0);
		QActive_disarm((QActive *) me);
		return Q_HANDLED();
	case Q_TIMEOUT_SIG:
		me->seconds++;
		if (me->seconds == pd.timelapse_rate) {
			// FIXME we could overflow the sutter queue if shutter is not done yet
			QActive_post((QActive *) &shutter_ao, SIG_SHUTTER_START, 0);
			me->seconds = 0;
		}
		QActive_arm((QActive *) me, TIMEOUT_SECOND);
		return Q_HANDLED();
	case SIG_PROG_STOP:
		return Q_TRAN(prog_idle);
	case SIG_SHUTTER_DONE:
		return Q_HANDLED();
	}

	return Q_SUPER(&QHsm_top);
}

static QState prog_timelapse_wait(struct prog_ao *me)
{

}


/**
 * Computes the field of view angle based on the focal length and the image size.
 * The field of view is computed by:
 *   a = 2 * arctan(d/2f)
 *   a = Field of view
 *   d = Image size
 *   f = Focal length
 * @param focal_length Focal length
 * @param size Image size
 * @return Returns the field of view angle in deg.
 */
static float compute_fov(float focal_length, float size)
{
	float f = focal_length;
	float d = size;
	float a;

	if (f == 0.0)
		return 0;

	a = atan(d / 2.0 * f) * 2.0;

	return a / PI * 180.0;
}
