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

/* Debounce reduces chatter (unintended double-presses) - set 0 if debouncing is not needed */
#define DEBOUNCE 10
