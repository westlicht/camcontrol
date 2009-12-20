
#ifndef __MENU_H__
#define __MENU_H__

#include <stdlib.h>
#include <stdint.h>

/** Menu node types */
typedef enum {
	MENU_TYP_ITEM,
	MENU_TYP_SUB,
	MENU_TYP_CMD,
	MENU_TYP_PARAM,
	MENU_TYP_LAST,
} menu_typ_t;

struct menu_item;
struct menu_param;

typedef const struct menu_item *menu_item_t;
typedef const struct menu_param *menu_param_t;

/** Menu item */
struct menu_item {
	menu_typ_t typ;
	const char *name;
	const struct menu_item *sub;
	union {
		struct {
			void (*handler)(void);	/**< Handler callback */
		} cmd;
		menu_param_t param;
	} u;
};

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

#define MENU_ITEM_PARAM(_name_, _param_) 				\
	.typ = MENU_TYP_PARAM,								\
	.name = _name_,										\
	.u.param = _param_

#define MENU_ITEM_LAST()								\
	.typ = MENU_TYP_LAST


struct menu_param {
	void *data;
	uint16_t min;
	uint16_t max;
	uint8_t step;
	uint8_t shift_step;
	int (*modify)(menu_item_t item, int dir, int shift);
	void (*print)(menu_item_t item);
	void (*changed)(menu_item_t item);
};

#define MENU_PARAM(_name_, _data_, _min_, _max_, _step_, _shift_step_, _modify_, _print_, _changed_) \
struct menu_param _name_ = {							\
	.data = _data_,										\
	.min = _min_,										\
	.max = _max_,										\
	.step = _step_,										\
	.shift_step = _shift_step_,							\
	.modify = _modify_,									\
	.print = _print_,									\
	.changed = _changed_,								\
};



/** Current menu item */
extern menu_item_t menu_cur;

void menu_init(void);

int menu_next(void);
int menu_prev(void);
int menu_sub(void);
int menu_parent(void);

#endif // __MENU_H__
