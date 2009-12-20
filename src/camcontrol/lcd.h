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
 * $Id: lcd.h,v 1.1 2005/12/28 21:38:59 joerg_wunsch Exp $
 */

/*
 * Initialize LCD controller.  Performs a software reset.
 */
void lcd_init(void);

#define LCD_FILL_BLANK	1

void lcd_clear(void);
void lcd_write(int x, int y, char *str, int flags);
