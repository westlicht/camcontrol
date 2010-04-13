/** @file menu.c
 *
 * Menu system.
 *
 * @author Simon Kallweit, simon@weirdsoft.ch
 */

#include <stddef.h>
#include "menu.h"

#include "menu_def.h"


#define MAX_DEPTH       8   /**< Max menu depth */

/** Menu position */
struct menu_pos {
    menu_item_t page;
    int item;
};

menu_item_t menu_cur;                           /**< Current menu item */
static struct menu_pos pos_stack[MAX_DEPTH];    /**< Menu position stack */
static int pos_index;                           /**< Current stack index */
static struct menu_pos *pos;                    /**< Current stack entry */

/**
 * Updates the menu_cur variable to point to the curret menu item.
 */
static inline void update_menu_cur(void)
{
    menu_cur = &pos->page[pos->item];
}

/**
 * Initializes the menu system.
 */
void menu_init(void)
{
    pos_index = 0;
    pos = &pos_stack[pos_index];
    pos->page = page_main;
    pos->item = 0;
    update_menu_cur();
}

/**
 * Navigates to the next menu item on the page.
 * @return Returns 1 if successful.
 */
int menu_next(void)
{
    if (pos->page[pos->item + 1].typ != MENU_TYP_LAST) {
        pos->item++;
        update_menu_cur();
        return 1;
    }
    return 0;
}

/**
 * Navigates to the previous menu item on the page.
 * @return Returns 1 if successful.
 */
int menu_prev(void)
{
    if (pos->item > 0) {
        pos->item--;
        update_menu_cur();
        return 1;
    }
    return 0;
}

/**
 * Navigates to the sub menu.
 * @return Returns 1 if successful.
 */
int menu_sub(void)
{
    if (pos->page[pos->item].typ == MENU_TYP_SUB && pos_index < MAX_DEPTH - 1) {
        pos_index++;
        pos_stack[pos_index].page = pos->page[pos->item].sub;
        pos_stack[pos_index].item = 0;
        pos = &pos_stack[pos_index];
        update_menu_cur();
        return 1;
    }
    return 0;
}

/**
 * Navigates to the parent menu.
 * @return Returns 1 if successful.
 */
int menu_parent(void)
{
    if (pos_index > 0) {
        pos_index--;
        pos = &pos_stack[pos_index];
        update_menu_cur();
        return 1;
    }
    return 0;
}
