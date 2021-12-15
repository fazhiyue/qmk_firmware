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
#include QMK_KEYBOARD_H
#include "print.h"
#include "quantum.h"
#include "oledfont.h"
#include "anime.h"
#include "bmp.h"

static uint32_t oled_timer = 0;		  // OLED 计时器
#define OLED_SHOW_STATE_TIMEOUT 20000 // 无操作10秒后激活OLED动画

#define ANIM_NUM_FRAMES 40	   // 动画总帧数
#define ANIM_FRAME_DURATION 20 // 帧时间
#define ANIM_TOTAL_FRAMES 79

#define OLED_SCL_Clr() writePinLow(B13) //SCL
#define OLED_SCL_Set() writePinHigh(B13)

#define OLED_SDA_Clr() writePinLow(B14) //SDA
#define OLED_SDA_Set() writePinHigh(B14)

#define OLED_RES_Clr() writePinLow(B15) //RES
#define OLED_RES_Set() writePinHigh(B15)

#define OLED_DC_Clr() writePinLow(A8) //DC
#define OLED_DC_Set() writePinHigh(A8)

//#define OLED_CS_Clr() writePinLow(C15) //CS
//#define OLED_CS_Set() writePinHigh(C15)

#define OLED_CMD 0	//写命令
#define OLED_DATA 1 //写数据

void OLED_ClearPoint(unsigned char x, unsigned char y);
void OLED_ColorTurn(unsigned char i);
void OLED_DisplayTurn(unsigned char i);
void OLED_WR_Byte(unsigned char dat, unsigned char mode);
void OLED_DisPlay_On(void);
void OLED_DisPlay_Off(void);
void OLED_Refresh(void);
void OLED_Clear(void);
void OLED_DrawPoint(unsigned char x, unsigned char y, unsigned char t);
void OLED_DrawLine(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2, unsigned char mode);
void OLED_DrawCircle(unsigned char x, unsigned char y, unsigned char r);
void OLED_ShowChar(unsigned char x, unsigned char y, unsigned char chr, unsigned char size1, unsigned char mode);
void OLED_ShowChar6x8(unsigned char x, unsigned char y, unsigned char chr, unsigned char mode);
void OLED_ShowString(unsigned char x, unsigned char y, unsigned char *chr, unsigned char size1, unsigned char mode);
void OLED_ShowNum(unsigned char x, unsigned char y, unsigned int num, unsigned char len, unsigned char size1, unsigned char mode);
void OLED_ShowChinese(unsigned char x, unsigned char y, unsigned char num, unsigned char size1, unsigned char mode);
void OLED_ScrollDisplay(unsigned char num, unsigned char space, unsigned char mode);
void OLED_ShowPicture(unsigned char x, unsigned char y, unsigned char sizex, unsigned char sizey, unsigned char BMP[], unsigned char mode);
void OLED_Init(void);

enum custom_keycodes
{
	JM_DOT = SAFE_RANGE, // . both shifted and unshifted
	JM_COMM,			 // , both shifted and unshifted
	JM_GRLS,			 // < and > on one key
	JM_CBRC,			 // { and } on one key
	JM_CENT,			 // cent sign and ! on one key
	JM_DPA1,			 // DUP/PA1,
	JM_FPA2,			 // FldMk/PA2,
	JM_JUMP,			 // JUMP/PA3,
	JM_DEL,				 // Delete
	JM_NSLC				 // NumLock/ScrollLock on one key
};

enum layer_names
{
	_BASE,
	_FN
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
	// PC/3270 layout: PC legends (blue)
	[_BASE] = LAYOUT(
		KC_ESC, KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6, KC_F7, KC_F8, KC_F9, KC_F10, KC_F11, KC_F12, KC_DEL, KC_PAUS, KC_NLCK,
		KC_GRV, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9, KC_0, KC_MINS, KC_EQL, KC_BSPC, KC_P7, KC_P8, KC_P9,
		KC_TAB, KC_Q, KC_W, KC_E, KC_R, KC_T, KC_Y, KC_U, KC_I, KC_O, KC_P, KC_LBRC, KC_RBRC, KC_BSLS, KC_P4, KC_P5, KC_P6,
		KC_CAPS, KC_A, KC_S, KC_D, KC_F, KC_G, KC_H, KC_J, KC_K, KC_L, KC_SCLN, KC_QUOT, KC_ENT, KC_P1, KC_P2, KC_P3,
		KC_LSFT, KC_Z, KC_X, KC_C, KC_V, KC_B, KC_N, KC_M, KC_COMM, KC_DOT, KC_SLSH, KC_RSFT, KC_UP, KC_P0,
		KC_LCTL, MO(1), KC_LGUI, KC_LALT, KC_SPC, KC_RALT, KC_APP, KC_RCTL, KC_LEFT, KC_DOWN, KC_RGHT, KC_PDOT),
	[_FN] = LAYOUT(
		RESET, RGB_HUI, RGB_SAI, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_INS, KC_LSCR, KC_NO,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, RGB_VAD, RGB_VAI, RGB_SPD, RGB_SPI, KC_TRNS, KC_HOME, KC_MS_U, KC_PGUP,
		KC_TAB, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_MS_L, KC_BTN1, KC_MS_R,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, RGB_MOD, KC_END, KC_MS_D, KC_PGDN,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_PGUP, KC_0,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, RGB_TOG, KC_TRNS, KC_TRNS, KC_TRNS, KC_HOME, KC_PGDN, KC_END, KC_TRNS),
};

unsigned char OLED_GRAM[144][8];

//反显函数
void OLED_ColorTurn(unsigned char i)
{
	if (i == 0)
	{
		OLED_WR_Byte(0xA6, OLED_CMD); //正常显示
	}
	if (i == 1)
	{
		OLED_WR_Byte(0xA7, OLED_CMD); //反色显示
	}
}

//屏幕旋转180度
void OLED_DisplayTurn(unsigned char i)
{
	if (i == 0)
	{
		OLED_WR_Byte(0xC8, OLED_CMD); //正常显示
		OLED_WR_Byte(0xA1, OLED_CMD);
	}
	if (i == 1)
	{
		OLED_WR_Byte(0xC0, OLED_CMD); //反转显示
		OLED_WR_Byte(0xA0, OLED_CMD);
	}
}

void OLED_WR_Byte(unsigned char dat, unsigned char cmd)
{
	unsigned char i;
	if (cmd)
		OLED_DC_Set();
	else
		OLED_DC_Clr();
	//OLED_CS_Clr();
	for (i = 0; i < 8; i++)
	{
		OLED_SCL_Clr();
		if (dat & 0x80)
			OLED_SDA_Set();
		else
			OLED_SDA_Clr();
		OLED_SCL_Set();
		dat <<= 1;
	}
	//OLED_CS_Set();
	OLED_DC_Set();
}

//开启OLED显示
void OLED_DisPlay_On(void)
{
	OLED_WR_Byte(0x8D, OLED_CMD); //电荷泵使能
	OLED_WR_Byte(0x14, OLED_CMD); //开启电荷泵
	OLED_WR_Byte(0xAF, OLED_CMD); //点亮屏幕
}

//关闭OLED显示
void OLED_DisPlay_Off(void)
{
	OLED_WR_Byte(0x8D, OLED_CMD); //电荷泵使能
	OLED_WR_Byte(0x10, OLED_CMD); //关闭电荷泵
	OLED_WR_Byte(0xAE, OLED_CMD); //关闭屏幕
}

//更新显存到OLED
void OLED_Refresh(void)
{
	unsigned char i, n;
	for (i = 0; i < 8; i++)
	{
		OLED_WR_Byte(0xb0 + i, OLED_CMD); //设置行起始地址
		OLED_WR_Byte(0x00, OLED_CMD);	  //设置低列起始地址
		OLED_WR_Byte(0x10, OLED_CMD);	  //设置高列起始地址
		for (n = 0; n < 128; n++)
			OLED_WR_Byte(OLED_GRAM[n][i], OLED_DATA);
	}
}
//清屏函数
void OLED_Clear(void)
{
	unsigned char i, n;
	for (i = 0; i < 8; i++)
	{
		for (n = 0; n < 128; n++)
		{
			OLED_GRAM[n][i] = 0; //清除所有数据
		}
	}
	OLED_Refresh(); //更新显示
}

//画点
//x:0~127
//y:0~63
//t:1 填充 0,清空
void OLED_DrawPoint(unsigned char x, unsigned char y, unsigned char t)
{
	unsigned char i, m, n;
	i = y / 8;
	m = y % 8;
	n = 1 << m;
	if (t)
	{
		OLED_GRAM[x][i] |= n;
	}
	else
	{
		OLED_GRAM[x][i] = ~OLED_GRAM[x][i];
		OLED_GRAM[x][i] |= n;
		OLED_GRAM[x][i] = ~OLED_GRAM[x][i];
	}
}

//画线
//x1,y1:起点坐标
//x2,y2:结束坐标
void OLED_DrawLine(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2, unsigned char mode)
{
	unsigned short t;
	int xerr = 0, yerr = 0, delta_x, delta_y, distance;
	int incx, incy, uRow, uCol;
	delta_x = x2 - x1; //计算坐标增量
	delta_y = y2 - y1;
	uRow = x1; //画线起点坐标
	uCol = y1;
	if (delta_x > 0)
		incx = 1; //设置单步方向
	else if (delta_x == 0)
		incx = 0; //垂直线
	else
	{
		incx = -1;
		delta_x = -delta_x;
	}
	if (delta_y > 0)
		incy = 1;
	else if (delta_y == 0)
		incy = 0; //水平线
	else
	{
		incy = -1;
		delta_y = -delta_x;
	}
	if (delta_x > delta_y)
		distance = delta_x; //选取基本增量坐标轴
	else
		distance = delta_y;
	for (t = 0; t < distance + 1; t++)
	{
		OLED_DrawPoint(uRow, uCol, mode); //画点
		xerr += delta_x;
		yerr += delta_y;
		if (xerr > distance)
		{
			xerr -= distance;
			uRow += incx;
		}
		if (yerr > distance)
		{
			yerr -= distance;
			uCol += incy;
		}
	}
}
//x,y:圆心坐标
//r:圆的半径
void OLED_DrawCircle(unsigned char x, unsigned char y, unsigned char r)
{
	int a, b, num;
	a = 0;
	b = r;
	while (2 * b * b >= r * r)
	{
		OLED_DrawPoint(x + a, y - b, 1);
		OLED_DrawPoint(x - a, y - b, 1);
		OLED_DrawPoint(x - a, y + b, 1);
		OLED_DrawPoint(x + a, y + b, 1);

		OLED_DrawPoint(x + b, y + a, 1);
		OLED_DrawPoint(x + b, y - a, 1);
		OLED_DrawPoint(x - b, y - a, 1);
		OLED_DrawPoint(x - b, y + a, 1);

		a++;
		num = (a * a + b * b) - r * r; //计算画的点离圆心的距离
		if (num > 0)
		{
			b--;
			a--;
		}
	}
}

//在指定位置显示一个字符,包括部分字符
//x:0~127
//y:0~63
//size1:选择字体 6x8/6x12/8x16/12x24
//mode:0,反色显示;1,正常显示
void OLED_ShowChar(unsigned char x, unsigned char y, unsigned char chr, unsigned char size1, unsigned char mode)
{
	unsigned char i, m, temp, size2, chr1;
	unsigned char x0 = x, y0 = y;
	if (size1 == 8)
		size2 = 6;
	else
		size2 = (size1 / 8 + ((size1 % 8) ? 1 : 0)) * (size1 / 2); //得到字体一个字符对应点阵集所占的字节数
	chr1 = chr - ' ';											   //计算偏移后的值
	for (i = 0; i < size2; i++)
	{
		if (size1 == 8)
		{
			temp = asc2_0806[chr1][i];
		} //调用0806字体
		else if (size1 == 12)
		{
			temp = asc2_1206[chr1][i];
		} //调用1206字体
		else if (size1 == 16)
		{
			temp = asc2_1608[chr1][i];
		} //调用1608字体
		else if (size1 == 24)
		{
			temp = asc2_2412[chr1][i];
		} //调用2412字体
		else
			return;
		for (m = 0; m < 8; m++)
		{
			if (temp & 0x01)
				OLED_DrawPoint(x, y, mode);
			else
				OLED_DrawPoint(x, y, !mode);
			temp >>= 1;
			y++;
		}
		x++;
		if ((size1 != 8) && ((x - x0) == size1 / 2))
		{
			x = x0;
			y0 = y0 + 8;
		}
		y = y0;
	}
}

//显示字符串
//x,y:起点坐标
//size1:字体大小
//*chr:字符串起始地址
//mode:0,反色显示;1,正常显示
void OLED_ShowString(unsigned char x, unsigned char y, unsigned char *chr, unsigned char size1, unsigned char mode)
{
	while ((*chr >= ' ') && (*chr <= '~')) //判断是不是非法字符!
	{
		if(x>=128)
		{
			x = 0;
			chr++;
			continue;
		}
		OLED_ShowChar(x, y, *chr, size1, mode);
		if (size1 == 8)
			x += 6;
		else
			x += size1 / 2;
		chr++;
	}
}

//m^n
unsigned int OLED_Pow(unsigned char m, unsigned char n)
{
	unsigned int result = 1;
	while (n--)
	{
		result *= m;
	}
	return result;
}

//显示数字
//x,y :起点坐标
//num :要显示的数字
//len :数字的位数
//size:字体大小
//mode:0,反色显示;1,正常显示
void OLED_ShowNum(unsigned char x, unsigned char y, unsigned int num, unsigned char len, unsigned char size1, unsigned char mode)
{
	unsigned char t, temp, m = 0;
	if (size1 == 8)
		m = 2;
	for (t = 0; t < len; t++)
	{
		temp = (num / OLED_Pow(10, len - t - 1)) % 10;
		if (temp == 0)
		{
			OLED_ShowChar(x + (size1 / 2 + m) * t, y, '0', size1, mode);
		}
		else
		{
			OLED_ShowChar(x + (size1 / 2 + m) * t, y, temp + '0', size1, mode);
		}
	}
}

//显示汉字
//x,y:起点坐标
//num:汉字对应的序号
//mode:0,反色显示;1,正常显示
void OLED_ShowChinese(unsigned char x, unsigned char y, unsigned char num, unsigned char size1, unsigned char mode)
{
	unsigned char m, temp;
	unsigned char x0 = x, y0 = y;
	unsigned short i, size3 = (size1 / 8 + ((size1 % 8) ? 1 : 0)) * size1; //得到字体一个字符对应点阵集所占的字节数
	for (i = 0; i < size3; i++)
	{
		if (size1 == 16)
		{
			temp = Hzk1[num][i];
		} //调用16*16字体
		else if (size1 == 24)
		{
			temp = Hzk2[num][i];
		} //调用24*24字体
		else if (size1 == 32)
		{
			temp = Hzk3[num][i];
		} //调用32*32字体
		else if (size1 == 64)
		{
			temp = Hzk4[num][i];
		} //调用64*64字体
		else
			return;
		for (m = 0; m < 8; m++)
		{
			if (temp & 0x01)
				OLED_DrawPoint(x, y, mode);
			else
				OLED_DrawPoint(x, y, !mode);
			temp >>= 1;
			y++;
		}
		x++;
		if ((x - x0) == size1)
		{
			x = x0;
			y0 = y0 + 8;
		}
		y = y0;
	}
}

//num 显示汉字的个数
//space 每一遍显示的间隔
//mode:0,反色显示;1,正常显示
void OLED_ScrollDisplay(unsigned char num, unsigned char space, unsigned char mode)
{
	unsigned char i, n, t = 0, m = 0, r;
	while (1)
	{
		if (m == 0)
		{
			OLED_ShowChinese(128, 24, t, 16, mode); //写入一个汉字保存在OLED_GRAM[][]数组中
			t++;
		}
		if (t == num)
		{
			for (r = 0; r < 16 * space; r++) //显示间隔
			{
				for (i = 1; i < 144; i++)
				{
					for (n = 0; n < 8; n++)
					{
						OLED_GRAM[i - 1][n] = OLED_GRAM[i][n];
					}
				}
				OLED_Refresh();
			}
			t = 0;
		}
		m++;
		if (m == 16)
		{
			m = 0;
		}
		for (i = 1; i < 144; i++) //实现左移
		{
			for (n = 0; n < 8; n++)
			{
				OLED_GRAM[i - 1][n] = OLED_GRAM[i][n];
			}
		}
		OLED_Refresh();
	}
}

//x,y：起点坐标
//sizex,sizey,图片长宽
//BMP[]：要写入的图片数组
//mode:0,反色显示;1,正常显示
void OLED_ShowPicture(unsigned char x, unsigned char y, unsigned char sizex, unsigned char sizey, unsigned char BMP[], unsigned char mode)
{
	unsigned short j = 0;
	unsigned char i, n, temp, m;
	unsigned char x0 = x, y0 = y;
	sizey = sizey / 8 + ((sizey % 8) ? 1 : 0);
	for (n = 0; n < sizey; n++)
	{
		for (i = 0; i < sizex; i++)
		{
			temp = BMP[j];
			j++;
			for (m = 0; m < 8; m++)
			{
				if (temp & 0x01)
					OLED_DrawPoint(x, y, mode);
				else
					OLED_DrawPoint(x, y, !mode);
				temp >>= 1;
				y++;
			}
			x++;
			if ((x - x0) == sizex)
			{
				x = x0;
				y0 = y0 + 8;
			}
			y = y0;
		}
	}
}
//OLED的初始化
void OLED_Init(void)
{

	//GPIO初始化设置
	setPinOutput(B13);
	setPinOutput(B15);
	setPinOutput(B14);
	setPinOutput(A8);

	OLED_RES_Clr();
	wait_ms(200);
	OLED_RES_Set();
	wait_ms(200);

	OLED_WR_Byte(0xAE, OLED_CMD); /*display off*/
	OLED_WR_Byte(0x00, OLED_CMD); /*set lower column address*/
	OLED_WR_Byte(0x10, OLED_CMD); /*set higher column address*/
	OLED_WR_Byte(0x00, OLED_CMD); /*set display start line*/
	OLED_WR_Byte(0xB0, OLED_CMD); /*set page address*/
	OLED_WR_Byte(0x81, OLED_CMD); /*contract control*/
	OLED_WR_Byte(0x8F, OLED_CMD); /*128*/
	OLED_WR_Byte(0xA1, OLED_CMD); /*set segment remap*/
	OLED_WR_Byte(0xA6, OLED_CMD); /*normal / reverse*/
	OLED_WR_Byte(0xA8, OLED_CMD); /*multiplex ratio*/
	OLED_WR_Byte(0x1F, OLED_CMD); /*duty = 1/32*/
	OLED_WR_Byte(0xC8, OLED_CMD); /*Com scan direction*/
	OLED_WR_Byte(0xD3, OLED_CMD); /*set display offset*/
	OLED_WR_Byte(0x00, OLED_CMD);
	OLED_WR_Byte(0xD5, OLED_CMD); /*set osc division*/
	OLED_WR_Byte(0x80, OLED_CMD);
	OLED_WR_Byte(0xD9, OLED_CMD); /*set pre-charge period*/
	OLED_WR_Byte(0x1f, OLED_CMD);
	OLED_WR_Byte(0xDA, OLED_CMD); /*set COM pins*/
	OLED_WR_Byte(0x00, OLED_CMD);
	OLED_WR_Byte(0xdb, OLED_CMD); /*set vcomh*/
	OLED_WR_Byte(0x40, OLED_CMD);
	OLED_WR_Byte(0x8d, OLED_CMD); /*set charge pump enable*/
	OLED_WR_Byte(0x14, OLED_CMD);
	OLED_WR_Byte(0xAF, OLED_CMD); /*display ON*/
}

void keyboard_pre_init_user(void)
{
	OLED_Init();
	OLED_Clear();
}

#define KEYLOGGER_LENGTH 7
static char keylog_str[KEYLOGGER_LENGTH + 1] = {0};

static const char PROGMEM code_to_name[0xFF] = {
	//   0    1    2    3    4    5    6    7    8    9    A    B    c    D    E    F
	182, ' ', ' ', ' ', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l',	  // 0x
	'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '1', '2',	  // 1x
	'3', '4', '5', '6', '7', '8', '9', '0', 20, 19, 17, 29, 22, '-', '=', '[',		  // 2x
	']', '\\', '#', ';', '\'', '`', ',', '.', '/', 188, 149, 150, 151, 152, 153, 154, // 3x
	155, 156, 157, 158, 159, 181, 191, 190, ' ', ' ', 185, 183, 16, 186, 184, 26,	  // 4x
	27, 25, 24, 189, '/', '*', '-', '+', ' ', '1', '2', '3', '4', '5', '6', '7',	  // 5x
	'8', '9', '0', '.', ' ', 187, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',	  // 6x
	' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',	  // 7x
	' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',	  // 8x
	' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',	  // 9x
	' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 214, 215, 216, 217, 218, 219, 220, 221,	  // Ax
	' ', ' ', 213, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',	  // Bx
	' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',	  // Cx
	' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',	  // Dx
	'C', 'S', 'A', 'W', ' ', 'S', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',	  // Ex
	' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '		  // Fx
};

void add_keylog(uint16_t keycode)
{
	if ((keycode >= QK_MOD_TAP && keycode <= QK_MOD_TAP_MAX) || (keycode >= QK_LAYER_TAP && keycode <= QK_LAYER_TAP_MAX) || (keycode >= QK_MODS && keycode <= QK_MODS_MAX))
	{
		keycode = keycode & 0xFF;
	}
	else if (keycode > 0xFF)
	{
		keycode = 0;
	}

	for (uint8_t i = (KEYLOGGER_LENGTH - 1); i > 0; --i)
	{
		keylog_str[i] = keylog_str[i - 1];
	}

	if (keycode < (sizeof(code_to_name) / sizeof(char)))
	{
		keylog_str[0] = pgm_read_byte(&code_to_name[keycode]);
	}
}
uint16_t anim_timer = 0;
int banner = 0;
int banner_total = 148;
int banner_reverse = 74;

void render_keylogger_status(void)
{
	OLED_ShowPicture(0, 0, 128, 32, BMP2, 0);
	OLED_ShowString(0, 0, (unsigned char *)&keylog_str, 24, 0);
	OLED_ShowString(abs((banner_reverse - 1) - banner), 24, (unsigned char *)"Love Rui!", 8, 0);
	if (timer_elapsed(anim_timer) > ANIM_FRAME_DURATION)
	{
		anim_timer = timer_read();
		banner = (banner + 1) % banner_total;
	}
	OLED_Refresh();
}


uint8_t current_anim_frame = 0;

void housekeeping_task_user(void)
{
	if (timer_elapsed32(oled_timer) > OLED_SHOW_STATE_TIMEOUT)
	{
		if (timer_elapsed(anim_timer) > ANIM_FRAME_DURATION)
		{
			anim_timer = timer_read();
			current_anim_frame = (current_anim_frame + 1) % ANIM_TOTAL_FRAMES;
			OLED_ShowPicture(0, 0, 128, 32, frame[abs((ANIM_NUM_FRAMES - 1) - current_anim_frame)], 1);
			OLED_Refresh();
			wait_ms(1);
			return;
		}
	}
	else
		render_keylogger_status();
	wait_ms(1);
}

bool process_record_user(uint16_t keycode, keyrecord_t *record)
{
	// If console is enabled, it will print the matrix position and status of each key pressed
	if (record->event.pressed)
	{
		add_keylog(keycode);
		oled_timer = timer_read32();
	}
	return true;
}
