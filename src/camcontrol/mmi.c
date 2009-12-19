#include "qpn_port.h"
#include "bsp.h"
#include "camcontrol.h"
#include "menu.h"
#include "key.h"
#include "lcd.h"

/** MMI active object structure */
struct mmi_ao {
	QActive super;
};

static QState mmi_initial(struct mmi_ao *me);
static QState mmi_hello(struct mmi_ao *me);
static QState mmi_navigate(struct mmi_ao *me);

static void update_screen(struct mmi_ao *me);

/** MMI active object */
struct mmi_ao mmi_ao;

enum timeouts {
	TIMEOUT_HELLO = TICKS(1000),
};

/**
 * Constructor.
 */
void mmi_ctor(void)
{
	QActive_ctor((QActive *) &mmi_ao, (QStateHandler) mmi_initial);
}

/**
 * Initial state.
 */
static QState mmi_initial(struct mmi_ao *me)
{
	menu_init();

	return Q_TRAN(mmi_hello);
}

/**
 * Hello state.
 * Prints a hello message and waits for keypress or timeout to switch to main menu.
 */
static QState mmi_hello(struct mmi_ao *me)
{
	switch (Q_SIG(me)) {
	case Q_ENTRY_SIG:
		// Print hello message
		lcd_write(0, 0, " CamControl 0.1");
		lcd_write(0, 1, "----------------");
		//QActive_arm((QActive *) me, TIMEOUT_HELLO);
		return Q_HANDLED();
	case Q_EXIT_SIG:
		// Clear screen
		QActive_disarm((QActive *) me);
		return Q_HANDLED();
	case Q_TIMEOUT_SIG:
	case SIG_ENCODER:
	case SIG_KEY_PRESS:
	case SIG_KEY_RELEASE:
		return Q_TRAN(mmi_navigate);
	}

	return Q_SUPER(&QHsm_top);
}

/**
 * Navigate state.
 * Navigates the menu structure using the following buttons:
 *  - up => go to previous item
 *  - down => go to next item
 *  - left => go to parent item
 *  - right => go to child item
 */
static QState mmi_navigate(struct mmi_ao *me)
{
	switch (Q_SIG(me)) {
	case Q_ENTRY_SIG:
		update_screen(me);
		return Q_HANDLED();
	case Q_EXIT_SIG:
		return Q_HANDLED();
	case SIG_ENCODER:
		if (Q_PAR(me) == 1) {
			if (menu_next())
				update_screen(me);
		} else {
			if (menu_prev())
				update_screen(me);
		}
		return Q_HANDLED();
	case SIG_KEY_PRESS:
		switch (Q_PAR(me)) {
		case KEY_UP:
			// Go to previous item
			if (menu_prev())
				update_screen(me);
			break;
		case KEY_DOWN:
			// Go to next item
			if (menu_next())
				update_screen(me);
			break;
		case KEY_LEFT:
			// Go to parent item
			if (menu_parent())
				update_screen(me);
			break;
		case KEY_RIGHT:
		case KEY_ENTER:
			// Go to sub item
			if (menu_sub())
				update_screen(me);
			break;
		}
		return Q_HANDLED();
	}

	return Q_SUPER(&QHsm_top);
}

static void update_screen(struct mmi_ao *me)
{
	lcd_clear();
	lcd_write(0, 0, menu_cur->name);
}