/** @file mmi.c
 *
 * MMI active object. Handles machine man interface.
 *
 * @author Simon Kallweit, simon@weirdsoft.ch
 */

#include <stdio.h>
#include "qpn_port.h"
#include "bsp.h"
#include "camcontrol.h"
#include "menu.h"
#include "key.h"
#include "lcd.h"
#include "debug.h"
#include "shutter.h"
#include "prog.h"
#include "servo.h"
#include "utils.h"
#include "backlight.h"
#include "mmi.h"

/** MMI active object structure */
struct mmi_ao {
    QActive super;
    int shift;
};

static QState mmi_initial(struct mmi_ao *me);
static QState mmi_hello(struct mmi_ao *me);
static QState mmi_navigate(struct mmi_ao *me);
static QState mmi_spherical_pan(struct mmi_ao *me);
static QState mmi_giga_pan(struct mmi_ao *me);
static QState mmi_show_msg(struct mmi_ao *me);
static QState mmi_busy(struct mmi_ao *me);

static QState execute_cmd(struct mmi_ao *me, int cmd);
static int modify_param(const struct param *param, int dir, int shift);
static void print_param(const struct param *param);

static void update_screen(struct mmi_ao *me);
static void show_msg(char *msg, int timeout);

/** MMI active object */
struct mmi_ao mmi_ao;

enum timeouts {
    TIMEOUT_HELLO = TICKS(5000),
    TIMEOUT_BUSY = TICKS(250),
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
        lcd_write(0, 0, " CamControl 0.1", 0);
        lcd_write(0, 1, "----------------", 0);
        QActive_post((QActive *) &backlight_ao, SIG_BACKLIGHT_ACTIVATE, 0);
        QActive_arm((QActive *) me, TIMEOUT_HELLO);
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
        switch (menu_cur->typ) {
        case MENU_TYP_PARAM:
            if (modify_param(menu_cur->param, Q_PAR(me), me->shift)) {
                print_param(menu_cur->param);
                if (menu_cur->cmd != CMD_NONE)
                    QActive_post((QActive *) me, SIG_MMI_CMD, menu_cur->cmd);
            }
            break;
        default:
            break;
        }
        return Q_HANDLED();
    case SIG_MMI_CMD:
        return execute_cmd(me, Q_PAR(me));
    case SIG_MMI_SHOW_MSG:
        return Q_TRAN(mmi_show_msg);
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
            // Go to sub item
            if (menu_sub())
                update_screen(me);
            break;
        case KEY_ENTER:
            me->shift = 1;
            switch (menu_cur->typ) {
            case MENU_TYP_CMD:
                // Execute command
                if (menu_cur->cmd)
                    QActive_post((QActive *) me, SIG_MMI_CMD, menu_cur->cmd);
                break;
            case MENU_TYP_SUB:
                // Go to sub item
                if (menu_sub())
                    update_screen(me);
                break;
            default:
                break;
            }
            break;
        }
        return Q_HANDLED();
    case SIG_KEY_RELEASE:
        switch (Q_PAR(me)) {
        case KEY_ENTER:
            me->shift = 0;
        default:
            break;
        }
        return Q_HANDLED();
    case SIG_PROG_START:
        return Q_TRAN(mmi_busy);
        break;
    }

    return Q_SUPER(&QHsm_top);
}

static QState mmi_spherical_pan(struct mmi_ao *me)
{
    char tmp[17];

    switch (Q_SIG(me)) {
    case Q_ENTRY_SIG:
        prog_init_spherical_pan();
        lcd_clear();
        lcd_write(0, 0, "Spherical pan", 0);
        snprintf(tmp, sizeof(tmp), "%d tiles", spherical_info.total_tiles);
        lcd_write(0, 1, tmp, 0);
        return Q_HANDLED();
    case Q_EXIT_SIG:
        return Q_HANDLED();
    case Q_TIMEOUT_SIG:
        return Q_HANDLED();
    case SIG_KEY_PRESS:
        switch (Q_PAR(me)) {
        case KEY_ENTER:
            QActive_post((QActive *) &prog_ao, SIG_PROG_START, PROG_SPHERICAL_PAN);
            return Q_TRAN(mmi_busy);
        case KEY_LEFT:
            return Q_TRAN(mmi_navigate);
        }
        return Q_HANDLED();;
    }

    return Q_SUPER(&QHsm_top);
}

static QState mmi_giga_pan(struct mmi_ao *me)
{
    char tmp[17];

    switch (Q_SIG(me)) {
    case Q_ENTRY_SIG:
        prog_init_giga_pan();
        lcd_clear();
        lcd_write(0, 0, "Giga pan", 0);
        snprintf(tmp, sizeof(tmp), "%dx%d tiles", giga_info.tiles.x, giga_info.tiles.y);
        lcd_write(0, 1, tmp, 0);
        return Q_HANDLED();
    case Q_EXIT_SIG:
        return Q_HANDLED();
    case Q_TIMEOUT_SIG:
        return Q_HANDLED();
    case SIG_KEY_PRESS:
        switch (Q_PAR(me)) {
        case KEY_ENTER:
            QActive_post((QActive *) &prog_ao, SIG_PROG_START, PROG_GIGA_PAN);
            return Q_TRAN(mmi_busy);
        case KEY_LEFT:
            return Q_TRAN(mmi_navigate);
        }
        return Q_HANDLED();;
    }

    return Q_SUPER(&QHsm_top);
}

static QState mmi_show_msg(struct mmi_ao *me)
{
    switch (Q_SIG(me)) {
    case Q_ENTRY_SIG:
        QActive_arm((QActive *) me, TICKS(Q_PAR(me) * 1000));
        return Q_HANDLED();
    case Q_EXIT_SIG:
        QActive_disarm((QActive *) me);
        return Q_HANDLED();
    case Q_TIMEOUT_SIG:
        return Q_TRAN(mmi_navigate);
    case SIG_KEY_PRESS:
        if (Q_PAR(me) == KEY_ENTER) {
            return Q_TRAN(mmi_navigate);
        }
        return Q_HANDLED();;
    }

    return Q_SUPER(&QHsm_top);
}

static QState mmi_busy(struct mmi_ao *me)
{
    static const char busy_char[] = ". ";
    static uint8_t busy_index;

    switch (Q_SIG(me)) {
    case Q_ENTRY_SIG:
        // Print hello message
        lcd_clear();
        lcd_write(0, 0, "Busy ...", 0);
        QActive_arm((QActive *) me, TIMEOUT_BUSY);
        return Q_HANDLED();
    case Q_EXIT_SIG:
        QActive_disarm((QActive *) me);
        return Q_HANDLED();
    case Q_TIMEOUT_SIG:
        lcd_char(15, 0, busy_char[busy_index]);
        busy_index = (busy_index + 1) % (sizeof(busy_char) - 1);
        QActive_arm((QActive *) me, TIMEOUT_BUSY);
        return Q_HANDLED();
    case SIG_KEY_PRESS:
        if (Q_PAR(me) == KEY_LEFT) {
            QActive_post((QActive *) &prog_ao, SIG_PROG_STOP, 0);
            return Q_TRAN(mmi_navigate);
        }
        return Q_HANDLED();
    case SIG_PROG_DONE:
        return Q_TRAN(mmi_navigate);
    }

    return Q_SUPER(&QHsm_top);
}

static QState execute_cmd(struct mmi_ao *me, int cmd)
{
    vec2f_t v;

    switch (cmd) {
    case CMD_SINGLE_SHOT:
        QActive_post((QActive *) &shutter_ao, SIG_SHUTTER_START, 0);
        break;
    case CMD_SPHERICAL_PAN:
        return Q_TRAN(mmi_spherical_pan);
    case CMD_GIGA_PAN:
        return Q_TRAN(mmi_giga_pan);
    case CMD_TIMELAPSE:
        QActive_post((QActive *) &prog_ao, SIG_PROG_START, PROG_TIMELAPSE);
        QActive_post((QActive *) &mmi_ao, SIG_PROG_START, PROG_TIMELAPSE);
        break;
    case CMD_SAVE:
        show_msg("Saving...", 1);
        param_save();
        break;
    case CMD_SERVO_MIN:
        vec2(&v, deg2rad(0.0), deg2rad(0.0));
        servo_move(&v);
        break;
    case CMD_SERVO_CENTER:
        vec2(&v, deg2rad(180.0), deg2rad(90.0));
        servo_move(&v);
        break;
    case CMD_SERVO_MAX:
        vec2(&v, deg2rad(360.0), deg2rad(180.0));
        servo_move(&v);
        break;
    case CMD_UPDATE_SERVO_MIN_X:
        servo_get_goal(&v);
        v.x = deg2rad(0);;
        servo_move(&v);
        break;
    case CMD_UPDATE_SERVO_MAX_X:
        servo_get_goal(&v);
        v.x = deg2rad(360);
        servo_move(&v);
        break;
    case CMD_UPDATE_SERVO_MIN_Y:
        servo_get_goal(&v);
        v.y = deg2rad(0);
        servo_move(&v);
        break;
    case CMD_UPDATE_SERVO_MAX_Y:
        servo_get_goal(&v);
        v.y = deg2rad(180);
        servo_move(&v);
        break;
    case CMD_UPDATE_CENTER:
        vec2(&v, deg2rad(pd.single.center_x), deg2rad(pd.single.center_y));
        servo_move(&v);
        break;
    case CMD_UPDATE_GIGA_START:
        vec2(&v, deg2rad(pd.giga.start_x), deg2rad(pd.giga.start_y));
        servo_move(&v);
        break;
    case CMD_UPDATE_GIGA_END:
        vec2(&v, deg2rad(pd.giga.end_x), deg2rad(pd.giga.end_y));
        servo_move(&v);
        break;
    case CMD_SHUTDOWN:
        vec2(&v, deg2rad(180.0), deg2rad(180.0));
        servo_move(&v);
        break;
    }

    return Q_HANDLED();
}

static int modify_param(const struct param *param, int dir, int shift)
{
    uint8_t step = shift ? param->class->shift_step : param->class->step;
    uint32_t value = param_get(param);

    if (dir == ENC_UP && value > param->class->min) {
        if (step > value - param->class->min)
            step = value - param->class->min;
        value -= step;
        param_set(param, value);
        return 1;
    }
    if (dir == ENC_DOWN && value < param->class->max) {
        if (step > param->class->max - value)
            step = param->class->max - value;
        value += step;
        param_set(param, value);
        return 1;
    }

    return 0;
}

static void print_param(const struct param *param)
{
    char tmp[17];

    param_print(menu_cur->param, tmp, sizeof(tmp));
    lcd_write(0, 1, tmp, LCD_FILL_BLANK);
}

static void update_screen(struct mmi_ao *me)
{
    lcd_clear();
    lcd_write(0, 0, (char *) menu_cur->name, 0);
    switch (menu_cur->typ) {
    case MENU_TYP_PARAM:
        print_param(menu_cur->param);
        break;
    default:
        break;
    }
}

static void show_msg(char *msg, int timeout)
{
    lcd_clear();
    lcd_write(0, 0, msg, 0);
    QActive_post((QActive *) &mmi_ao, SIG_MMI_SHOW_MSG, timeout);
}
