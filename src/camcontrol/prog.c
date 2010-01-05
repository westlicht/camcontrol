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
#include "utils.h"
#include "mmi.h"

/** Program active object structure */
struct prog_ao {
	QActive super;
	uint32_t seconds;
};

static QState prog_initial(struct prog_ao *me);
static QState prog_idle(struct prog_ao *me);
static QState prog_spherical_pan(struct prog_ao *me);
static QState   prog_spherical_pan_step(struct prog_ao *me);
static QState prog_giga_pan(struct prog_ao *me);
static QState   prog_giga_pan_step(struct prog_ao *me);
static QState prog_timelapse(struct prog_ao *me);
static QState   prog_timelapse_wait(struct prog_ao *me);

static void init_camera(void);
static float compute_fov(float focal_length, float size, float crop);
static void compute_spherical_row(struct spherical_info *info);

/** Program active object */
struct prog_ao prog_ao;

struct camera_info camera_info;
struct spherical_info spherical_info;
struct giga_info giga_info;

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
	struct spherical_info *info = &spherical_info;
	float t;

	init_camera();

	DBG("Spherical pan parameters:\n");

	// Compute number of rows
	t = M_PI - MIN(camera_info.fov.x, camera_info.fov.y);
	info->tiles.y = 2 + (ceil(t / camera_info.fov.y));
	DBG("rows %d\n", info->tiles.y);

	// Compute vertical step size
	info->step.y = t / (info->tiles.y - 2);
	DBG("step (normalized) %.2f/%.2f\n", rad2deg(info->step.x), rad2deg(info->step.y));

	// Compute vertical origin
	info->origin = (M_PI - (camera_info.fov.y * (info->tiles.y - 3))) / 2.0;
	DBG("origin %f\n", rad2deg(info->origin));

	// Compute total number of tiles
	info->total_tiles = 0;
	for (info->index.y = 0; info->index.y  < info->tiles.y; info->index.y++) {
		compute_spherical_row(info);
		info->total_tiles += info->tiles.x;
	}
	DBG("total tiles %d\n", info->total_tiles);

	// Prepare 1st row
	info->index.y = 0;
	compute_spherical_row(info);

	return 1;
}

/**
 * Prepares the giga pan parameters.
 * @return Returns 1 if parameters are valid.
 */
int prog_init_giga_pan(void)
{
	struct giga_info *info = &giga_info;
	vec2f_t size;

	init_camera();

	DBG("Giga pan parameters:\n");

	// Compute size of panorama
	size.x = deg2rad(pd.giga.end_x - pd.giga.start_x);
	size.y = deg2rad(pd.giga.end_y - pd.giga.start_y);
	DBG("size %.2f/%.2f\n", rad2deg(size.x), rad2deg(size.y));

	// Compute number of tiles
	if (size.x <= camera_info.fov.x) {
		info->tiles.x = 1;
	} else {
		info->tiles.x = ceil(fabs(size.x / camera_info.fov.x));
	}
	if (size.y <= camera_info.fov.y) {
		info->tiles.y = 1;
	} else {
		info->tiles.y = ceil(fabs(size.y / camera_info.fov.y));
	}
	DBG("tiles %d/%d\n", info->tiles.x, info->tiles.y);

	// Compute step size
	info->step.x = size.x / info->tiles.x;
	info->step.y = size.y / info->tiles.y;
	DBG("step %.2f/%.2f\n", rad2deg(info->step.x), rad2deg(info->step.y));

	// Compute origin
	info->origin.x = deg2rad(pd.giga.start_x);
	info->origin.y = deg2rad(pd.giga.start_y);
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
		QActive_post((QActive *) me, SIG_PROG_STEP, 0);
		return Q_HANDLED();
	case Q_EXIT_SIG:
		return Q_HANDLED();
	case Q_TIMEOUT_SIG:
		return Q_HANDLED();
	case SIG_PROG_STEP:
		return Q_TRAN(prog_spherical_pan_step);
	}

	return Q_SUPER(&QHsm_top);
}

static QState prog_spherical_pan_step(struct prog_ao *me)
{
	struct spherical_info *info = &spherical_info;

	switch (Q_SIG(me)) {
	case Q_ENTRY_SIG:
		// Move servos into position
		DBG("move to %d/%d (%.2f/%.2f)\n", info->index.x, info->index.y, rad2deg(info->pos.x), rad2deg(info->pos.y));
		servo_move(&info->pos);
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
		info->index.x++;
		if (info->index.x == info->tiles.x) {
			info->index.y++;
			if (info->index.y >= info->tiles.y) {
				QActive_post((QActive *) &mmi_ao, SIG_PROG_DONE, 0);
				return Q_TRAN(prog_idle);
			}
			compute_spherical_row(info);
		} else {
			info->pos.x += info->step.x;
		}
		QActive_post((QActive *) me, SIG_PROG_STEP, 0);
		return Q_HANDLED();
	}

	return Q_SUPER(&prog_spherical_pan);
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
	struct giga_info *info = &giga_info;
	vec2f_t pos;

	switch (Q_SIG(me)) {
	case Q_ENTRY_SIG:
		// Move servos into position
		vec2(&pos, info->origin.x + info->index.x * info->step.x,
				   info->origin.y + info->index.y * info->step.y);
		DBG("move to %d/%d (%.2f/%.2f)\n", info->index.x, info->index.y, rad2deg(pos.x), rad2deg(pos.y));
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
		info->index.x++;
		if (info->index.x == info->tiles.x) {
			info->index.x = 0;
			info->index.y++;
			if (info->index.y >= info->tiles.y) {
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


static void init_camera(void)
{
	struct camera_info *info = &camera_info;
	float overlap;

	DBG("Camera info:\n");

	// Compute field of view
	info->fov.x = compute_fov(pd.focal_length / 10.0, pd.sensor_width / 10.0, pd.crop / 100.0);
	info->fov.y = compute_fov(pd.focal_length / 10.0, pd.sensor_height / 10.0, pd.crop / 100.0);
	overlap = deg2rad(pd.giga.overlap / 100.0);
	info->fov.x -= overlap;
	info->fov.y -= overlap;
	DBG("fov %.2f/%.2f\n", rad2deg(info->fov.x), rad2deg(info->fov.y));
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
 * @param crop Crop factor
 * @return Returns the field of view angle in radians.
 */
static float compute_fov(float focal_length, float size, float crop)
{
	float f = focal_length;
	float d = size / crop;
	float a;

	if (f == 0.0)
		return 0;

	a =  2.0 * atan(d / (f * 2.0));

	return a;
}

static void compute_spherical_row(struct spherical_info *info)
{
	float a;

	if (info->index.y == 0 ) {
		// Top row
		info->tiles.x = 1;
		vec2(&info->pos, deg2rad(0.0), deg2rad(0.0));
	} else if (info->index.y == info->tiles.y - 1) {
		// Bottom row
		info->tiles.x = 1;
		vec2(&info->pos, deg2rad(360.0), deg2rad(180.0));
	} else if (info->index.y < info->tiles.y / 2) {
		// Upper half row
		vec2(&info->pos, 0.0, info->origin + (info->index.y - 1) * info->step.y);
		a = info->pos.y + camera_info.fov.y / 2.0;
		info->tiles.x = ceil(M_2PI / (camera_info.fov.x / sin(a)));
		info->step.x = M_2PI / info->tiles.x;
	} else {
		// Lower half row
		vec2(&info->pos, 0.0, info->origin + (info->index.y - 1) * info->step.y);
		a = info->pos.y - camera_info.fov.y / 2.0;
		info->tiles.x = ceil(M_2PI / (camera_info.fov.x / sin(a)));
		info->step.x = M_2PI / info->tiles.x;
	}

	DBG("pos %.2f/%.2f\n", rad2deg(info->pos.x), rad2deg(info->pos.y));
	DBG("cols %d\n", info->tiles.x);

	info->index.x = 0;
}
