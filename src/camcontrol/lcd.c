/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <joerg@FreeBSD.ORG> wrote this file.  As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.        Joerg Wunsch
 * ----------------------------------------------------------------------------
 *
 * Stdio demo, upper layer of LCD driver.
 *
 * $Id: lcd.c,v 1.1 2005/12/28 21:38:59 joerg_wunsch Exp $
 */

#include "defines.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include <avr/io.h>

#include <util/delay.h>

#include "hd44780.h"
#include "lcd.h"

/*
 * Setup the LCD controller.  First, call the hardware initialization
 * function, then adjust the display attributes we want.
 */
void lcd_init(void)
{
  hd44780_init();

  /*
   * Clear the display.
   */
  hd44780_outcmd(HD44780_CLR);
  hd44780_wait_ready();

  /*
   * Entry mode: auto-increment address counter, no display shift in
   * effect.
   */
  hd44780_outcmd(HD44780_ENTMODE(1, 0));
  hd44780_wait_ready();

  /*
   * Enable display, deactivate cursor.
   */
  hd44780_outcmd(HD44780_DISPCTL(1, 0, 0));
  hd44780_wait_ready();
}

/**
 * Clears the LCD.
 */
void lcd_clear(void)
{
	hd44780_wait_ready();
	hd44780_outcmd(HD44780_CLR);
	hd44780_wait_ready();
	hd44780_outcmd(HD44780_HOME);
}

/**
 * Writes to the LCD at position [x, y]. If LCD_FILL_BLANK is set in the flags,
 * writes blanks to the end of the line.
 * @param x X position
 * @param y Y position
 * @param str String to print
 * @param flags Flags
 */
void lcd_write(int x, int y, char *str, int flags)
{
	hd44780_wait_ready();
	hd44780_outcmd(HD44780_DDADDR((y * 40 + x)));
	while (*str != '\0' && x++ < 16) {
		hd44780_wait_ready();
		hd44780_outdata(*str++);
	}
	if (flags & LCD_FILL_BLANK)
		while (x++ < 16) {
			hd44780_wait_ready();
			hd44780_outdata(' ');
		}
}

/**
 * Writes a single character at position [x, y].
 * @param x X position
 * @param y Y position
 * @param c Character to print
 */
void lcd_char(int x, int y, char c)
{
	hd44780_wait_ready();
	hd44780_outcmd(HD44780_DDADDR((y * 40 + x)));
	hd44780_wait_ready();
	hd44780_outdata(c);
}
