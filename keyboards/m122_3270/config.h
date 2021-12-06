/* Copyright 2021 James R. Maynard III <jaymaynard@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include "config_common.h"

#define VENDOR_ID 0xFEED
#define PRODUCT_ID 0x6060
#define DEVICE_VER 0x0002
#define MANUFACTURER QMK
#define PRODUCT BajiePG1350
#define DESCRIPTION STM32F411 based.

// Both controllers draw 100 mA or less
#define USB_MAX_POWER_CONSUMPTION 500

/* key matrix size */
#define MATRIX_ROWS 6
#define MATRIX_COLS 17
#define RGBLED_NUM 92
#define DRIVER_LED_TOTAL  92
#define RGB_DI_PIN C13
#define RGB_MATRIX_KEYPRESSES

#define EECONFIG_RGB_MATRIX (uint32_t *)28

#define RGB_MATRIX_MAXIMUM_BRIGHTNESS 200     // 最大亮度限制
#define RGB_MATRIX_STARTUP_VAL RGB_MATRIX_MAXIMUM_BRIGHTNESS
//#define RGB_MATRIX_STARTUP_MODE RGB_MATRIX_SOLID_REACTIVE_NEXUS
/* COL2ROW, ROW2COL*/
#define DIODE_DIRECTION ROW2COL

/* OLED */
#ifdef OLED_DRIVER_ENABLE
#    define OLED_DISPLAY_CUSTOM
#    define OLED_DISPLAY_WIDTH 128
#    define OLED_DISPLAY_HEIGHT 32

#    define I2C_DRIVER I2CD1
//#    define OLED_IC OLED_IC_SH1106  // 选用1.3寸SH1106主控OLED时打开, 0.96寸SSD1306主控OLED注释掉本行。
//#    define OLED_COLUMN_OFFSET 2  // 调整OLED纵向偏移，0.96寸SSD1306主控OLED注释掉本行。
#    define I2C1_TIMINGR_PRESC 0U
#    define I2C1_TIMINGR_SCLDEL 7U
#    define I2C1_TIMINGR_SDADEL 0U
#    define I2C1_TIMINGR_SCLH 38U
#    define I2C1_TIMINGR_SCLL 129U
#    define OLED_TIMEOUT 60000  // 60秒无操作关闭OLED
#    define OLED_FONT_HEIGHT 8
#    define OLED_FONT_WIDTH 6
#    define OLED_FONT_H "neil_font.c"
#endif

/* Debounce reduces chatter (unintended double-presses) - set 0 if debouncing is not needed */
#define DEBOUNCE 10
