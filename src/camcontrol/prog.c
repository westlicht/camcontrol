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
#include "servo.h"
#include "mmi.h"

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
static QState   prog_giga_pan_step(struct prog_ao *me);
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
 * @return Returns 1 if parameters are valid.
 */
int prog_init_spherical_pan(void)
{
	//spherical_pan.fov_x = 10.0;
	//spherical_pan.fov_y = 15.0;
	spherical_pan.tiles_x = 10;
	spherical_pan.tiles_y = 5;

	return 0;
}

/**
 * Prepares the giga pan parameters.
 * @return Returns 1 if parameters are valid.
 */
int prog_init_giga_pan(void)
{
	struct giga_pan *info = &giga_pan;
	vec2f_t fov;
	vec2f_t size;
	float overlap;

	DBG("Giga pan parameters:\n");

	// Compute field of view
	fov.x = compute_fov(pd.focal_length / 10.0, pd.sensor_width / 10.0);
	fov.y = compute_fov(pd.focal_length / 10.0, pd.sensor_height / 10.0);
	DBG("fov %.2f/%.2f\n", fov.x, fov.y);

	// Compute step size
	overlap = pd.giga.overlap / 100.0;
	info->step.x = fov.x - (overlap * 2.0);
	info->step.y = fov.y - (overlap * 2.0);
	DBG("step %.2f/%.2f\n", info->step.x, info->step.y);
	if (info->step.x < 0.0 || info->step.y < 0.0)
		return 0;

	// Compute size of panorama
	size.x = (pd.giga.end_x - pd.giga.start_x);
	size.y = (pd.giga.end_y - pd.giga.start_y);
	DBG("size %.2f/%.2f\n", size.x, size.y);

	// Compute number of tiles
	if (size.x <= fov.x) {
		info->tiles.x = 1;
	} else {
		info->tiles.x = ceil(fabs(size.x / info->step.x));
	}
	if (size.y <= fov.y) {
		info->tiles.y = 1;
	} else {
		info->tiles.y = ceil(fabs(size.y / info->step.y));
	}
	DBG("tiles %d/%d\n", info->tiles.x, info->tiles.y);

	// Recompute step size
	info->step.x = size.x / info->tiles.x;
	info->step.y = size.y / info->tiles.y;
	DBG("step (normalized) %.2f/%.2f\n", info->step.x, info->step.y);

	// Compute origin
	info->origin.x = pd.giga.start_x;
	info->origin.y = pd.giga.start_y;
	info->index.x = 0;
	info->index.y = 0;

	return 1;
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
		QActive_post((QActive *) me, SIG_PROG_STEP, 0);
		return Q_HANDLED();
	case Q_EXIT_SIG:
		return Q_HANDLED();
	case Q_TIMEOUT_SIG:
		return Q_HANDLED();
	case SIG_PROG_STEP:
		return Q_TRAN(prog_giga_pan_step);
	}

	return Q_SUPER(&QHsm_top);
}

static QState prog_giga_pan_step(struct prog_ao *me)
{
	vec2f_t pos;

	switch (Q_SIG(me)) {
	case Q_ENTRY_SIG:
		// Move servos into position
		vec2(&pos, giga_pan.origin.x + giga_pan.index.x * giga_pan.step.x,
				   giga_pan.origin.y + giga_pan.index.y * giga_pan.step.y);
		DBG("move to %d/%d (%.2f/%.2f)\n", giga_pan.index.x, giga_pan.index.y, pos.x, pos.y);
		servo_move(&pos);
		return Q_HANDLED();
	case Q_EXIT_SIG:
		return Q_HANDLED();
	case Q_TIMEOUT_SIG:
		return Q_HANDLED();
	case SIG_SERVO_DONE:
		// Servos are in position
		DBG("servo moved\n");
		// Shut image
		DBG("shut\n");
		QActive_post((QActive *) &shutter_ao, SIG_SHUTTER_START, 0);
		return Q_HANDLED();
	case SIG_SHUTTER_DONE:
		// Image is shut
		DBG("shut done\n");
		// Prepare next step
		giga_pan.index.x++;
		if (giga_pan.index.x == giga_pan.tiles.x) {
			giga_pan.index.x = 0;
			giga_pan.index.y++;
			if (giga_pan.index.y >= giga_pan.tiles.y) {
				QActive_post((QActive *) &mmi_ao, SIG_PROG_DONE, 0);
				return Q_TRAN(prog_idle);
			}
		}
		QActive_post((QActive *) me, SIG_PROG_STEP, 0);
		return Q_HANDLED();
	}

	return Q_SUPER(&prog_giga_pan);
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

	a = 2.0 * atan(d / (f * 2.0));

	return a / PI * 180.0;
}
