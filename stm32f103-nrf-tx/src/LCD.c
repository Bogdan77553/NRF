/*
**
**                           Main.c
**
**
**********************************************************************/
/*
   Last committed:     $Revision: 00 $
   Last changed by:    $Author: $
   Last changed date:  $Date:  $
   ID:                 $Id:  $

**********************************************************************/
#include "LCD.h"
//#include "font.h"




void initLCD(void)
{
//uint16_t DeviceCode=0;
  	 WriteData(0xffff);
   	 delay(100); // 10 ms
  	GPIO_SetBits(GPIOE, GPIO_Pin_1);

   // DeviceCode = ReadData(0x0000);

   //************* Start Initial Sequence **********//
main_W_com_data(0x00E5, 0x78F0);
main_W_com_data(0x0001, 0x0100); // set SS and SM bit
main_W_com_data(0x0002, 0x0700); // set 1 line inversion
main_W_com_data(0x0003, 0x1018); // set GRAM write direction and BGR=1.
main_W_com_data(0x0004, 0x0000); // Resize register
main_W_com_data(0x0008, 0x0207); // set the back porch and front porch
main_W_com_data(0x0009, 0x0000); // set non-display area refresh cycle ISC[3:0]
main_W_com_data(0x000A, 0x0000); // FMARK function
main_W_com_data(0x000C, 0x0000); // RGB interface setting
main_W_com_data(0x000D, 0x0000); // Frame marker Position
main_W_com_data(0x000F, 0x0000); // RGB interface polarity

//*************Power On sequence ****************//

main_W_com_data(0x0010, 0x0000); // SAP, BT[3:0], AP, DSTB, SLP, STB
main_W_com_data(0x0011, 0x0007); // DC1[2:0], DC0[2:0], VC[2:0]
main_W_com_data(0x0012, 0x0000); // VREG1OUT voltage
main_W_com_data(0x0013, 0x0000); // VDV[4:0] for VCOM amplitude
main_W_com_data(0x0007, 0x0001);
delay(200); // Dis-charge capacitor power voltage
main_W_com_data(0x0010, 0x1090); // SAP, BT[3:0], AP, DSTB, SLP, STB
main_W_com_data(0x0011, 0x0227); // Set DC1[2:0], DC0[2:0], VC[2:0]
delay(50); // Delay 50ms
main_W_com_data(0x0012, 0x001F); // 0012
delay(50); // Delay 50ms
main_W_com_data(0x0013, 0x1200); // VDV[4:0] for VCOM amplitude
main_W_com_data(0x0029, 0x0025); // 04  VCM[5:0] for VCOMH
main_W_com_data(0x002B, 0x000E); // Set Frame Rate
delay(50); // Delay 50ms
main_W_com_data(0x0020, 0x0000); // GRAM horizontal Address
main_W_com_data(0x0021, 0x0000); // GRAM Vertical Address

// ----------- Adjust the Gamma Curve ----------//

main_W_com_data(0x0030, 0x0000);
main_W_com_data(0x0031, 0x0707);
main_W_com_data(0x0032, 0x0307);
main_W_com_data(0x0035, 0x0200);
main_W_com_data(0x0036, 0x0008);
main_W_com_data(0x0037, 0x0004);
main_W_com_data(0x0038, 0x0000);
main_W_com_data(0x0039, 0x0707);
main_W_com_data(0x003C, 0x0504);
main_W_com_data(0x003D, 0x0808);
//------------------ Set GRAM area ---------------//
main_W_com_data(0x0050, 0x0000); // Horizontal GRAM Start Address
main_W_com_data(0x0051, 0x00EF); // Horizontal GRAM End Address
main_W_com_data(0x0052, 0x0000); // Vertical GRAM Start Address
main_W_com_data(0x0053, 0x013F); // Vertical GRAM Start Address
main_W_com_data(0x0060, 0xA700); // Gate Scan Line
main_W_com_data(0x0061, 0x0001); // NDL,VLE, REV
main_W_com_data(0x006A, 0x0000); // set scrolling line

//-------------- Partial Display Control ---------//

main_W_com_data(0x0080, 0x0000);
main_W_com_data(0x0081, 0x0000);
main_W_com_data(0x0082, 0x0000);
main_W_com_data(0x0083, 0x0000);
main_W_com_data(0x0084, 0x0000);
main_W_com_data(0x0085, 0x0000);

//-------------- Panel Control -------------------//

main_W_com_data(0x0090, 0x0010);
main_W_com_data(0x0092, 0x0000);
main_W_com_data(0x0007, 0x0133); // 262K color and display ON

}



/*******************************************************************/

void delay(unsigned long ms)// delay 1 ms per count @ Crystal 8.0 MHz and PLL9x or SYSCLK = 72 MHz
{
    volatile unsigned long i,j;
    for (i = 0; i < ms; i++ )
    for (j = 0; j < 5525; j++ );
}
/*******************************************************************/
void main_W_com_data(uint16_t reg, uint16_t dat){
LCD_REG =reg;
LCD_DATA=dat;
}
/*******************************************************************/
void  WriteData(uint16_t data) {
	LCD_DATA=data;
}
/*******************************************************************/
void WriteCommand(uint16_t index)
{
  LCD_REG = index;
}
uint16_t ReadData(uint8_t indx)
{
 uint16_t temp;
  WriteCommand(indx);
  temp=LCD_DATA;
   return temp;
}


void setOrientation(unsigned int HV)//horizontal or vertical
{
    WriteCommand(0x0003);
    if(HV==1)//vertical
    {
         WriteData(0x1018);

    }
    else//horizontal
    {
         WriteData(0x1030);
    }
  WriteCommand(0x0022); //Start to write to display RAM
}
void TFT_set_cursor(unsigned int x_pos, unsigned int y_pos)
{
    unsigned int temp = 0x0000;

    #if (DISP_ORIENTATION == 0)

    #elif (DISP_ORIENTATION == 90)
    {
        temp = x_pos;
        x_pos = y_pos;
        y_pos = (MAX_X - temp - 1);
    }
    #elif (DISP_ORIENTATION == 180)
    {
        x_pos = (MAX_X - x_pos - 1);
        y_pos = (MAX_Y - y_pos - 1);
    }
    #elif (DISP_ORIENTATION == 270)
    {
        temp = y_pos;
        y_pos = x_pos;
        x_pos = (MAX_Y - temp - 1);
    }
    #endif




    main_W_com_data(0x0020, x_pos);
    main_W_com_data(0x0021, y_pos);
    WriteCommand(0x0022); //Start to write to display RAM
}

void TFT_set_display_window(unsigned int x_pos, unsigned int y_pos, unsigned int w, unsigned int h)
{
   main_W_com_data(0x0050, x_pos);
    main_W_com_data(0x0051, (w + x_pos));
    main_W_com_data(0x0052,y_pos);
   main_W_com_data(0x0053,(h + y_pos));
    TFT_set_cursor(x_pos, y_pos);
}

//ô-öèß ðèñóåò òî÷êó çàäàííîãî ðàçìåðà
void Draw_Point (unsigned int x,unsigned int y,unsigned int color,uint8_t size)
{
	uint8_t j,i = 0;

	for(j=0;j < size; j++)
	{
		TFT_set_cursor(x, y+j);
		for(i=0;i < size;i++)
		{
			 WriteData(color);
		}
	}
}

//warning
void LCD_clear(uint16_t color)
{ unsigned int i,j;
    TFT_set_display_window(0,0,240,320);
    for(i=0;i<=320;i++)
	{
	    for(j=0;j<=240;j++)
		{
            WriteData(color);
		}
	}


}

void setPixel(unsigned int poX, unsigned int poY,uint16_t color)
{

    TFT_set_cursor(poX,poY);
    WriteData(color);

}
//ô-öèß ðèñóåò ëèíèþ ïî Àëãîðèòì Áðåçåíõåìà
void Draw_Line (unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2,unsigned int color)
{
	unsigned int deltaX = abs(x2 - x1);
	unsigned int deltaY = abs(y2 - y1);
	unsigned int signX = x1 < x2 ? 1 : -1;
	unsigned int signY = y1 < y2 ? 1 : -1;
	unsigned int error = deltaX - deltaY;
	unsigned int error2 = error * 2;

	for (;;)
	{
		setPixel(x1,y1,color);

		if(x1 == x2 && y1 == y2)
		break;



		if(error2 > -deltaY)
		{
			error -= deltaY;
			x1 += signX;
		}

		if(error2 < deltaX)
		{
			error += deltaX;
			y1 += signY;
		}
	}
}

/////////////////////////////////
//ô-öèß ðèñóåò ãîðèçîíòàëüíóþ ëèíèþ
void Draw_Horizont_Line(unsigned int x1,unsigned int y1,unsigned int y2,unsigned int color)
{
	Draw_Line(x1, y1, x1, y2, color);
}
///////////////////////////////
//ô-öèß ðèñóåò âåðòèêàëüíóþ ëèíèþ
void Draw_Vertical_Line(unsigned int x1,unsigned int x2,unsigned int y1,unsigned int color)
{
	Draw_Line(x1, y1, x2, y1, color);
}
///////////////////////////////
//ô-öèß ðèñóåò ïðßìîóãîëüíèê
void Draw_Reactangle(unsigned int left,unsigned int top,unsigned int right,unsigned int bottom,unsigned int color)
{
	Draw_Horizont_Line(top, left, right, color);
	Draw_Horizont_Line(bottom, left, right, color);
	Draw_Vertical_Line(top, bottom, left, color);
	Draw_Vertical_Line(top, bottom, right, color);

}
////////////////////////////////////
///////////////////////////////////////
//ô-öèß çàêðàøèâàåò âûáðàííóþ îáëàñòü
void Draw_Area(unsigned int left, unsigned int top, unsigned int right, unsigned int bottom, unsigned int color)
{
	unsigned int x,y;

	TFT_set_display_window(left, top, right, bottom);

	for(y=top; y<=bottom; y++)
	{
		for(x=left; x<=right; x++)
		{
			  WriteData(color);
	    }
	}

	TFT_set_display_window(0, 0, 320, 240 );

}



void drawCircle(int poX, int poY, int r,unsigned int color)
{
    int x = -r, y = 0, err = 2-2*r, e2;
    do {
        setPixel(poX-x, poY+y,color);
        setPixel(poX+x, poY+y,color);
        setPixel(poX+x, poY-y,color);
        setPixel(poX-x, poY-y,color);
        e2 = err;
        if (e2 <= y) {
            err += ++y*2+1;
            if (-x == y && e2 <= x) e2 = 0;
        }
        if (e2 > x) err += ++x*2+1;
    }
    while (x <= 0);
}

void fillCircle(int poX, int poY, int r,unsigned int color)
{
    int x = -r, y = 0, err = 2-2*r, e2;
    do {

        drawVerticalLine(poX-x,poY-y,2*y,color);
        drawVerticalLine(poX+x,poY-y,2*y,color);

        e2 = err;
        if (e2 <= y) {
            err += ++y*2+1;
            if (-x == y && e2 <= x) e2 = 0;
        }
        if (e2 > x) err += ++x*2+1;
    }
    while (x <= 0);

}


void drawLine(unsigned int x0,unsigned int y0,unsigned int x1,unsigned int y1,unsigned int color)
{
    int dx = (x1-x0), sx = x0<x1 ? 1 : -1;
    int dy = -(y1-y0), sy = y0<y1 ? 1 : -1;
    int err = dx+dy, e2; // error value e_xy
    for (;;){ // loop
        setPixel(x0,y0,color);
        e2 = 2*err;
        if (e2 >= dy) { // e_xy+e_x > 0
            if (x0 == x1) break;
            err += dy;
            x0 += sx;
        }
        if (e2 <= dx) { // e_xy+e_y < 0
            if (y0 == y1) break;
            err += dx;
            y0 += sy;
        }
    }
}


void drawVerticalLine(unsigned int poX, unsigned int poY,unsigned int length,unsigned int color)
{
	unsigned int i;
	if(poX <0)
		poX = 0;
	if(poX>239)
		poX = 240;

	if(poY <0)
		poY = 0;
	if(poY>319)
		poY = 320;
    //poX = constrain(poX,MIN_X,MAX_X); //Limits the pixel range to 0 - 239
    //poY = constrain(poY,MIN_Y,MAX_Y); //Limits the pixel range to 0 - 319

    TFT_set_cursor(poX,poY);
    //setOrientation(0);
    if(length+poY>MAX_Y)
    {
        length=MAX_Y-poY;
    }

    for(i=0;i<length;i++)
    {
         WriteData(color);
    }
}

void  drawHorizontalLine(unsigned int poX, unsigned int poY,unsigned int length,uint16_t color)
{
	unsigned int i;
	if(poX <0)
		poX = 0;
	if(poX>320)
		poX = 320;

	if(poY <0)
		poY = 0;
	if(poY>240)
		poY = 240;
    //poX = constrain(poX,MIN_X,MAX_X); //Limits the pixel range to 0 - 239
    //poY = constrain(poY,MIN_Y,MAX_Y); //Limits the pixel range to 0 - 319

    TFT_set_cursor(poX,poY);
    //setOrientation(0);
    if(length+poX>MAX_X)
    {
        length=MAX_X-poX;
    }
    for(i=0;i<length;i++)
    {
         WriteData(color);

    }

}


void drawRectangle(unsigned int poX, unsigned int poY, unsigned int length,unsigned int width,unsigned int color)
{
    drawHorizontalLine(poX, poY, length, color);
    drawHorizontalLine(poX, poY+width, length, color);

    drawVerticalLine(poX, poY, width,color);
    drawVerticalLine(poX + length, poY, width,color);
}

void fillRectangle(unsigned int poX, unsigned int poY, unsigned int length, unsigned int width, uint16_t color)
{
	unsigned int i;
    for(i=0;i<width;i++)
    {
        drawHorizontalLine(poX, poY+i, length, color);
    }
}

void drawChar(unsigned char ascii,unsigned int poX, unsigned int poY,unsigned int size, unsigned int fgcolor)
{
	unsigned char i;
	unsigned char temp;
	unsigned char f;
    TFT_set_cursor(poX,poY);
    //setOrientation(1);
    if((ascii < 0x20)||(ascii > 0x7e))//Unsupported char.
    {
        ascii = '?';
    }
    for(i=0;i<8;i++)
    {
        temp = simpleFont[ascii-0x20][i];
        for(f=0;f<8;f++)
        {
            if((temp>>f)&0x01)
            {
                fillRectangle(poX+i*size, poY+f*size, size, size, fgcolor);
            }

        }
    }
}

void drawString(char *string,unsigned int poX, unsigned int poY,unsigned int size,unsigned int fgcolor)
{
    while(*string)
    {
         drawChar(*string, poX, poY, size, fgcolor);
         string++;

        if(poX < MAX_X)
        {
            poX+=8*size; // Move cursor right
        }
    }
}

unsigned char drawNumber(long long_num,unsigned int poX, unsigned int poY,unsigned int size,unsigned int fgcolor)
{
  unsigned char char_buffer[10]="";
  unsigned char i = 0;
  unsigned char f = 0;

  if (long_num < 0)
  {
    f=1;
    drawChar(0,poX, poY, size, fgcolor);
    long_num = -long_num;
    if(poX < MAX_X)
    {
        poX+=8*size; // Move cursor right
    }
  }
  else if (long_num == 0)
  {
    f=1;

    drawChar(0,poX, poY, size, fgcolor);
    return f;
    if(poX < MAX_X)
    {
        poX+=8*size; // Move cursor right
    }
  }


  while (long_num > 0)
  {
    char_buffer[i++] = long_num % 10;
    long_num /= 10;
  }

  f=f+i;
  for(; i > 0; i--)
  {

      //PrintSevenSegNumFont(6,poX, poY, size, fgcolor);
    drawChar('0'+ char_buffer[i - 1],poX, poY, size, fgcolor);
    if(poX < MAX_X)
    {
        poX+=8*size; // Move cursor right
    }
  }
  return f;
}

unsigned char drawFloat(float floatNumber,unsigned char decimal,unsigned int poX, unsigned int poY,unsigned int size,unsigned int fgcolor)
{
  unsigned int temp=0;
  float decy=0.0;
  float rounding = 0.5;
  unsigned char f=0;
  unsigned char i;
  unsigned char howlong;

  if(floatNumber<0.0)
  {
    drawChar('-',poX, poY, size, fgcolor);
    floatNumber = -floatNumber;
    if(poX < MAX_X)
    {
        poX+=8*size; // Move cursor right
    }
    f =1;
  }
  for (i=0; i<decimal; ++i)
  {
    rounding /= 10.0;
  }
  floatNumber += rounding;

  temp = (unsigned int)floatNumber;
  howlong=drawNumber(temp,poX, poY, size, fgcolor);
  f += howlong;
  if((poX+8*size*howlong) < MAX_X)
    {
        poX+=8*size*howlong; // Move cursor right
    }

  if(decimal>0)
  {
    drawChar('.',poX, poY, size, fgcolor);
    if(poX < MAX_X)
    {
        poX+=8*size; // Move cursor right
    }
    f +=1;
 }
  decy = floatNumber-temp;//decimal part,
  for(i=0;i<decimal;i++)//4
  {
    decy *=10;// for the next decimal
    temp = decy;//get the decimal
    drawNumber(temp,poX, poY, size, fgcolor);
    floatNumber = -floatNumber;
    if(poX < MAX_X)
    {
        poX+=8*size; // Move cursor right
    }
    decy -= temp;
  }
  f +=decimal;
  return (unsigned char)f;
}
///////////////////////////////////////////

void PrintSevenSegNumFont(uint8_t N, unsigned int X, unsigned int Y,unsigned int z, unsigned int clr){

   volatile unsigned int  b, i,j,q=0,jj=0;
   volatile unsigned char d;

for(b=0; b<50; b++)
{
  TFT_set_cursor(X,(Y+b));
  for(i=0; i<4; i++)
  {
     d=SevenSegNumFont[N][q];

    for(j=8; j>0; j--){

        if( d & (1 << (j-1)) ){
                fillRectangle(X+jj*z, Y+b*z, z, z, clr);

            // WriteData(clr);
        }else{
        fillRectangle(X+jj*z, Y+b*z, z, z, BLACK);
        }

        jj++;
    }

    q++;
  }
  jj=0;

}
}

void Draw_BMP(unsigned int x_pos, unsigned int y_pos, unsigned int H, unsigned int W, unsigned char *bitmap)
{
     unsigned long index = 0;
     unsigned long size = (W * H);
     unsigned int *bitmap_ptr = (unsigned int*)bitmap;

     TFT_set_display_window(x_pos, y_pos, (W - 1), (H - 1));
     WriteCommand(0x0022);//Start to write to display RAM

     for(index = 0; index < size; index++)
     {
         WriteData(*bitmap_ptr);
         bitmap_ptr++;

     }


}


