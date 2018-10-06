
#ifndef LCD_H
#define LCD_H

#include "stm32f10x_conf.h"
#include "font.h"


#define CS_LOW GPIO_ResetBits(GPIOD, GPIO_Pin_7);
#define WR_LOW GPIO_ResetBits(GPIOD, GPIO_Pin_5);
#define REST_LOW GPIO_ResetBits(GPIOE, GPIO_Pin_1);

#define CS_HIGH GPIO_SetBits(GPIOD, GPIO_Pin_7);
#define WR_HIGH GPIO_SetBits(GPIOD, GPIO_Pin_5);
#define REST_HIGH GPIO_SetBits(GPIOE, GPIO_Pin_1);


unsigned int IC_CODE; //Stores Driver IC ID (either SPFD5408A  or ST7781R)

//Basic Colors
#define RED 		0xf800
#define BRIGHT_RED	0xf810
#define GREEN0		0x7e00
#define GREEN1		0x07E0
#define GREEN2		0xB723
#define GREEN3		0x8000
#define BLUE0		0x001f
#define BLUE1		0x051D
#define BLACK		0x0000
#define YELLOW0		0xffe0
#define YELLOW1		0xFFE0
#define ORANGE		0xFBE4
#define WHITE		0xffff
#define CYAN		0x07ff
#define GRAY1		0x8410
#define GRAY2		0x4208
#define GRAY3		0x7BEF
#define MAGENTA1    0xF81F
#define MAGENTA2	0xA254
#define BROWN		0xBBCA

//#define DISP_ORIENTATION                  0
#define DISP_ORIENTATION               90
//#define DISP_ORIENTATION             180
//#define DISP_ORIENTATION          270

#if (DISP_ORIENTATION == 90) || (DISP_ORIENTATION == 270)

#define  MAX_X        320
#define  MAX_Y        240

#elif (DISP_ORIENTATION == 0) || (DISP_ORIENTATION == 180)

#define  MAX_X        240
#define  MAX_Y        320

#endif
#define  X_CONST     76800
#define WINDOW_XADDR_START	0x0050 // Horizontal Start Address Set
#define WINDOW_XADDR_END	0x0051 // Horizontal End Address Set
#define WINDOW_YADDR_START	0x0052 // Vertical Start Address Set
#define WINDOW_YADDR_END	0x0053 // Vertical End Address Set
#define GRAM_XADDR		    0x0020 // GRAM Horizontal Address Set
#define GRAM_YADDR		    0x0021 // GRAM Vertical Address Set
#define GRAMWR 			    0x0022 // memory write

// Äëÿ çàïèñè äàííûõ
#define  LCD_DATA    			    (*(uint16_t*)0x60020000)
// Äëÿ çàïèñè êîìàíä
#define  LCD_REG   		  	    (*(uint16_t*)0x60000000)
#define LCDFSA_dataREG 0x22
void delay(unsigned long ms);// delay 1 ms per count @ Crystal 8.0 MHz and PLL9x or SYSCLK = 72 MHz
void main_W_com_data(uint16_t reg, uint16_t dat);

/*******************************************************************/
void writeLCDData(uint16_t data);
/*******************************************************************/
uint16_t ReadData(uint8_t indx);
void WriteCommand(uint16_t index);
void initLCD(void);






void LCD_clear(uint16_t color);
void Draw_Point (unsigned int,unsigned int,unsigned int,uint8_t);
void Draw_Line (unsigned int,unsigned int,unsigned int,unsigned int ,unsigned int );
void Draw_Horizont_Line(unsigned intx1,unsigned int y1,unsigned inty2,unsigned int color);
void Draw_Vertical_Line(unsigned int x1,unsigned intx2,unsigned int y1,unsigned int color);
void Draw_Reactangle(unsigned int left,unsigned int top,unsigned int right,unsigned int bottom,unsigned int color);
void Draw_Area(unsigned int left, unsigned int top, unsigned int right, unsigned int bottom, unsigned int color);

void PrintSevenSegNumFont(uint8_t N, unsigned int X, unsigned int Y,unsigned int z, unsigned int clr);




void TFT_set_cursor(unsigned int poX, unsigned int poY);
void TFT_set_display_window(unsigned int x_pos, unsigned int y_pos, unsigned int w, unsigned int h);
void Draw_BMP(unsigned int x_pos, unsigned int y_pos, unsigned int H, unsigned int W, unsigned char *bitmap);

void setPixel(unsigned int poX, unsigned int poY,uint16_t color);
void drawLine(unsigned int x0,unsigned int y0,unsigned int x1,unsigned int y1,unsigned int color);
void drawVerticalLine(unsigned int poX, unsigned int poY,unsigned int length,unsigned int color);
void drawHorizontalLine(unsigned int poX, unsigned int poY,unsigned int length,uint16_t color);
void drawRectangle(unsigned int poX, unsigned int poY, unsigned int length,unsigned int width,unsigned int color);
void fillRectangle(unsigned int poX, unsigned int poY, unsigned int length, unsigned int width, uint16_t color);
void drawCircle(int poX, int poY, int r,unsigned int color);
void fillCircle(int poX, int poY, int r,unsigned int color);
void drawChar(unsigned char ascii,unsigned int poX, unsigned int poY,unsigned int size, unsigned int fgcolor);
void drawString(char *string,unsigned int poX, unsigned int poY,unsigned int size,unsigned int fgcolor);
unsigned char drawNumber(long long_num,unsigned int poX, unsigned int poY,unsigned int size,unsigned int fgcolor);

unsigned char drawFloat(float floatNumber,unsigned char decimal,unsigned int poX, unsigned int poY,unsigned int size,unsigned int fgcolor);


void setOrientation(unsigned int HV);

GPIO_InitTypeDef		LCDCTR_Setup;
GPIO_InitTypeDef		LCDIO_Setup;

//static int color = YELLOW;



 /**
     *
     *
     * @param
     * @param
     */

void CONFIG_LCD_PINS(void);
#endif/* LCD_H */
