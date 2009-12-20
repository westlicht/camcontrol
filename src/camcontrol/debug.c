
#include <stdio.h>

#include "lcd.h"
#include "delay.h"
#include "debug.h"


void on_assert(const char *filename, uint16_t line)
{
	char tmp[17];

	lcd_init();
	lcd_clear();
	snprintf(tmp, sizeof(tmp), "ASSERT (%d)", line);
	lcd_write(0, 0, tmp, 0);
	snprintf(tmp, sizeof(tmp), "%s", filename);
	lcd_write(0, 1, tmp, 0);

	while (1) {
		DBG_LED_ON();
		delay_ms(500);
		DBG_LED_OFF();
		delay_ms(500);
	}
}
