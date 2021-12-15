/* Copyright 2021 James R. Maynard III
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

#include "m122_3270.h"
#ifdef RGB_MATRIX_ENABLE
#define XX NO_LED
led_config_t g_led_config = {  //
    {
        // Key Matrix to LED Index
        {91, 90, 89, 88, 87, 86, 85, 84, 83, 82, 81, 80, 79, 78, 77, 76, XX},        //
        {59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75},  //
        {58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43, 42},  //
        {26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, XX, 38, 39, 40, 41},  //
        {25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, XX, 14, XX, 13, 12, XX},  //
        { 0,  1,  2,  3, XX, XX,  4, XX, XX,  5,  6, XX,  7,  8,  9, 10, 11},  // !! 轴灯对应矩阵的接线顺序
    },
    {
        // LED Index to Physical Position                                                                                                                                        
        {2, 3},{14, 3},{28, 3},{42, 3},{84, 3}, {126, 3},{140, 3}, {168, 3},{182, 3},{196, 3},{210, 3},{224, 3},   
        {210, 16},{196, 16},{168, 16},{140, 16},{126, 16},{112, 16},{98, 16},{84, 16},{70, 16},{56, 16},{42, 16},{28, 16},{14, 16},{2, 16},
        {2, 29},{14, 29},{28, 29},{42, 29},{56, 29},{70, 29},{84, 29},{98, 29},{112, 29},{126, 29},{140, 29},{154, 29},{182, 29},{196, 29},{210, 29},{224, 29},
        {224, 40},{210, 40},{196, 40},{182, 40},{168, 40},{154, 40},{140, 40},{126, 40},{112, 40},{98, 40},{84, 40},{70, 40},{56, 40},{42, 40},{28, 40},{14, 40},{2, 40},
        {2, 52},{14, 52},{28, 52},{42, 52},{56, 52},{70, 52},{84, 52},{98, 52},{112, 52},{126, 52},{140, 52},{154, 52},{168, 52},{182, 52},{196, 52},{210, 52},{224, 52},
        {210, 64},{196, 64},{182, 64},{168, 64},{154, 64},{140, 64},{126, 64},{112, 64},{98, 64},{84, 64},{70, 64},{56, 64},{42, 64},{28, 64},{14, 64},{2, 64},             
        //!! 按接线顺序排列轴灯的物理位置
    },

    {
        // LED Index to Flag
        4, 4, 4, 4,4, 4, 4, 4,4, 4, 4, 4,4, 4, 4, 4,4, 4, 4, 4,4, 4, 4, 4,4, 4, 4, 4,4, 4, 4, 4,4, 4, 4, 4,4, 4, 4, 4,4, 4, 4, 4,4, 4, 4, 4,4, 4, 4, 4,4, 4, 4, 4,4, 4, 4, 4,4, 4, 4, 4,4, 4, 4, 4,4, 4, 4, 4,4, 4, 4, 4,4, 4, 4, 4,4, 4, 4, 4,4, 4, 4, 4,4, 4, 4, 3,
    }};
#endif