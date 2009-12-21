#include "qpn_port.h"
#include "bsp.h"
#include "camcontrol.h"
#include "globals.h"
#include "lcd.h"
#include "key.h"
#include "debug.h"
#include "servo.h"
#include "delay.h"
#include "shutter.h"

/* Event queues */
static QEvent mmi_queue[8];
static QEvent prog_queue[8];

/* QF_active[] array defines all active object control blocks --------------*/
QActiveCB const Q_ROM Q_ROM_VAR QF_active[] = {
	{ (QActive *) 0,			(QEvent *) 0,	0						},
	{ (QActive *) &mmi_ao,		mmi_queue,		Q_DIM(mmi_queue)		},
	{ (QActive *) &prog_ao,		prog_queue,		Q_DIM(prog_queue)		},
};

/* Make sure that the QF_active[] array matches QF_MAX_ACTIVE in qpn_port.h */
Q_ASSERT_COMPILE(QF_MAX_ACTIVE == Q_DIM(QF_active) - 1);


int main (void)
{
	cli();

	bsp_init();

	lcd_init();
	key_init();
	servo_init();
	shutter_init();

	// Load from EEPROM or use default globals
	if (globals_load() != 0)
		globals_default();

	mmi_ctor();
	prog_ctor();

	QF_run();
}
