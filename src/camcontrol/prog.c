#include "qpn_port.h"
#include "bsp.h"
#include "camcontrol.h"
#include "debug.h"
#include "shutter.h"
#include "prog.h"
#include "param.h"

/** Program active object structure */
struct prog_ao {
	QActive super;
	uint32_t seconds;
};

static QState prog_initial(struct prog_ao *me);
static QState prog_idle(struct prog_ao *me);
static QState prog_timelapse(struct prog_ao *me);
static QState   prog_timelapse_wait(struct prog_ao *me);

/** Program active object */
struct prog_ao prog_ao;

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
		case PROG_TIMELAPSE:
			return Q_TRAN(prog_timelapse);
		}
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
