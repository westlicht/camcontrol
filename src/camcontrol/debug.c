/** @file debug.c
 *
 * Debug support.
 *
 * @author Simon Kallweit, simon@weirdsoft.ch
 */

#include <stdio.h>

#include "lcd.h"
#include "debug.h"

#include "defines.h"
#include <util/delay.h>

/**
 * Shows an assertion on the LCD and enters infinite blinking led loop.
 * @param filename Filename of assert
 * @param line Line number of assert
 */
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
		_delay_ms(500);
		DBG_LED_OFF();
		_delay_ms(500);
	}
}
