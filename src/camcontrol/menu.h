
#ifndef __MENU_H__
#define __MENU_H__

#include <stdlib.h>

/** Menu node types */
typedef enum {
	MENU_TYP_ITEM,
	MENU_TYP_SUB,
	MENU_TYP_CMD,
	MENU_TYP_LAST,
} menu_typ_t;

/** Parameter types */
typedef enum {
	PARAM_TYP_SENSOR_DIM,
	PARAM_TYP_DEG,
} param_typ_t;

/** Menu item */
struct menu_item {
	menu_typ_t typ;
	const char *name;
	const struct menu_item *sub;
	union {
		struct {
			void (*handler)(void);	/**< Handler callback */
		} cmd;
		struct {
			param_typ_t typ;
			void *data;
		} param;
	} u;
};

typedef const struct menu_item *menu_item_t;

#define MENU_ITEM(_name_)								\
	.typ = MENU_TYP_ITEM,								\
	.name = _name_,										\
	.sub = NULL

#define MENU_ITEM_SUB(_name_, _sub_)					\
	.typ = MENU_TYP_SUB,								\
	.name = _name_,										\
	.sub = _sub_

#define MENU_ITEM_CMD(_name_, _handler_)				\
	.typ = MENU_TYP_CMD,								\
	.name = _name_,										\
	.sub = NULL,										\
	.u.cmd.handler = _handler_

#define MENU_ITEM_LAST()								\
	.typ = MENU_TYP_LAST

#define MENU_PARAM(_typ_, _data_)						\
	.typ = MENU_TYP_PARAM,								\
	.u.param.typ = _typ_,								\
	.u.param.data = _data_

/** Current menu item */
extern menu_item_t menu_cur;

void menu_init(void);

int menu_next(void);
int menu_prev(void);
int menu_sub(void);
int menu_parent(void);

#endif // __MENU_H__
