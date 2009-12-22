
#ifndef __MENU_H__
#define __MENU_H__

#include <stdlib.h>
#include <stdint.h>
#include "param.h"
#include "mmi.h"

/** Menu node types */
typedef enum {
	MENU_TYP_ITEM,
	MENU_TYP_SUB,
	MENU_TYP_CMD,
	MENU_TYP_PARAM,
	MENU_TYP_LAST,
} menu_typ_t;

struct menu_item;

typedef const struct menu_item *menu_item_t;

/** Menu item */
struct menu_item {
	menu_typ_t typ;
	const char *name;
	const struct menu_item *sub;
	enum command cmd;
	const struct param *param;
};

#define MENU_ITEM(_name_)								\
	.typ = MENU_TYP_ITEM,								\
	.name = _name_,										\
	.sub = NULL

#define MENU_ITEM_SUB(_name_, _sub_)					\
	.typ = MENU_TYP_SUB,								\
	.name = _name_,										\
	.sub = _sub_

#define MENU_ITEM_CMD(_name_, _cmd_)					\
	.typ = MENU_TYP_CMD,								\
	.name = _name_,										\
	.sub = NULL,										\
	.cmd = _cmd_

#define MENU_ITEM_PARAM(_name_, _param_, _cmd_) 		\
	.typ = MENU_TYP_PARAM,								\
	.name = _name_,										\
	.cmd = _cmd_,										\
	.param = _param_

#define MENU_ITEM_LAST()								\
	.typ = MENU_TYP_LAST




/** Current menu item */
extern menu_item_t menu_cur;

void menu_init(void);

int menu_next(void);
int menu_prev(void);
int menu_sub(void);
int menu_parent(void);

#endif // __MENU_H__
