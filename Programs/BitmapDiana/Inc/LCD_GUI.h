/*****************************************************************************
* | File      	:	LCD_GUI.h
* | Author      :   Waveshare team
* | Function    :	Achieve drawing: draw points, lines, boxes, circles and
*                   their size, solid dotted line, solid rectangle hollow
*					rectangle, solid circle hollow circle.
* | Info        :
*   Achieve display characters: Display a single character, string, number
*   Achieve time display: adaptive size display time minutes and seconds
*----------------
* |	This version:   V1.0
* | Date        :   2017-08-16
* | Info        :   Basic version
*
******************************************************************************/

// General rule: The position of a coordinate is always checked. However, 
//               pixels lying beyond the edge of the screen are not shown.

/****************************Upper application layer**************************/
#ifndef __LCD_GUI_H
#define __LCD_GUI_H

#include "LCD_general.h"
#include "fonts.h"
#include <stdbool.h>

/********************************************************************************
function:
			dot pixel
********************************************************************************/
typedef enum {
    DOT_PIXEL_1X1  = 1,	// dot pixel 1 x 1
    DOT_PIXEL_2X2  , 		// dot pixel 2 X 2
    DOT_PIXEL_3X3  ,		// dot pixel 3 X 3
    DOT_PIXEL_4X4  ,		// dot pixel 4 X 4
    DOT_PIXEL_5X5  , 		// dot pixel 5 X 5
    DOT_PIXEL_6X6  , 		// dot pixel 6 X 6
    DOT_PIXEL_7X7  , 		// dot pixel 7 X 7
    DOT_PIXEL_8X8  , 		// dot pixel 8 X 8
} DOT_PIXEL;

/********************************************************************************
function:
			dot Fill style
********************************************************************************/
typedef enum {
    DOT_FILL_AROUND  = 1,		
    DOT_FILL_RIGHTUP  ,
} DOT_STYLE;


/********************************************************************************
function:
			Defines commonly used colors for the display
********************************************************************************/
#define LCD_BACKGROUND		WHITE   //Default background color
#define FONT_BACKGROUND		WHITE   //Default font background color
#define FONT_FOREGROUND	  GRED    //Default font foreground color

#define WHITE          0xFFFF
#define BLACK          0x0000
#define BLUE           0x001F
#define BRED           0XF81F
#define GRED           0XFFE0
#define GBLUE          0X07FF
#define RED            0xF800
#define MAGENTA        0xF81F
#define GREEN          0x07E0
#define CYAN           0x7FFF
#define YELLOW         0xFFE0
#define BROWN          0XBC40
#define BRRED          0XFC07
#define GRAY           0X8430

#define DEFAULT_BRIGHTNESS 800

/**
  * @brief  Initialisation of the LCD display.
  * @param  blVal     LCD display brightness
  * @retval none
  */
extern void GUI_init(uint16_t blVal);

/**
  * @brief  Clear LCD Display.
  * @param  color     Background color
  * @retval none
  */
extern void GUI_clear(COLOR color);

/**
  * @brief  This function draws a point as follows:
  *   if dotStyle == DOT_FILL_RIGHTUP :
  *      cord is the top left point af a quadrat of size IxI, 
	*       where dotPixel == DOT_PIXEL_IXI
  *   if dotStyle == DOT_FILL_AROUND :
  *      cord is the center af a quadrat of size (2*(I-1)+1)x(2*(I-1)+1),
	*       where dotPixel == DOT_PIXEL_IXI
	*   Pixels outside the area of the screen are not printed.
  *
  * @param  crd      Coordinate of the point
  * @param  color    Color of the point
  * @param  dotPixel Size of the point
  * @param  dotStyle This parameter defines the style and center of the point
  * @retval none
  */
extern void GUI_drawPoint(Coordinate crd, COLOR color, DOT_PIXEL dotPixel, DOT_STYLE dotStyle);

/**
  * @brief This function draws a rectangle.
	*        Pixels outside the area of the screen are not printed.
  *
  * @param  tl         Top left pixel (!!) of the rectangle
  * @param  br         Bottom right pixel of the rectangle
  * @param  color      Color of the rectangle
  * @param  filled     determines whether the rectangle is filled
  * @param  lineWeight Line weight of the rectangle 
  * @retval none
  */
extern void GUI_drawRectangle(Coordinate tl, Coordinate br, COLOR color, bool filled, DOT_PIXEL lineWeight);

/**
  * @brief  This function draws line.
	*         Pixels outside the area of the screen are not printed.
  *   if dotStyle == DOT_FILL_RIGHTUP :
  *      cord is the top left point af a quadrat of size IxI, 
	*       where dotPixel == DOT_PIXEL_IXI
  *   if dotStyle == DOT_FILL_AROUND :
  *      cord is the center af a quadrat of size (2*(I-1)+1)x(2*(I-1)+1),
	*       where dotPixel == DOT_PIXEL_IXI
	*   Pixels outside the area of the screen are not printed.
  *
  * @param  left      Left point of the line
  * @param  right     Right point of the line
  * @param  fg        Color of line
  * @param  bg        Background color (for dotted lines)
	* @param  solid     Solid or dotted
	*                   Dotted mode is supported for horizontal and vertical lines only.
  * @param  lineWidth Line Width
  * @retval none
  */
extern void GUI_drawLine(Coordinate left, Coordinate right, COLOR fg, COLOR bg, bool solid, DOT_PIXEL lineWidth);

/**
  * @brief This function draws a cycle.
	*        Pixels outside the area of the screen are not printed.
  *
  * @param  center     Center of the cycle
  * @param  color      Color of the cycle
  * @param  filled     determines whether the cycle is filled
  * @param  lineWeight Line weight of the cycle 
  * @retval none
  */
extern void GUI_drawCircle(Coordinate center, LENGTH radius, COLOR color, bool filled , DOT_PIXEL lineWeight);

/**
  * @brief This function draws a character of a STM32 font.
	*        Pixels outside the area of the screen are not printed.
  *
  * @param  tl         Top left pixel of the character
  * @param  ch         Character to be printed
  * @param  font       STM32 font to be used
  * @param  bg         Background color
  * @param  fg         Foreground color
  * @retval none
  */
extern void GUI_disChar(Coordinate tl, char ch, sFONT* font, COLOR bg, COLOR fg);

/**
  * @brief This function displays a string.
	*        If the string does not fit into a line, a newline will be inserted.
  *
  * @param  tl         Top left pixel of the string
  * @param  s          String to be displayed (including newline)
  * @param  font       STM32 font to be used
  * @param  bg         Background color
  * @param  fg         Foreground color
  * @retval none
  */
extern void GUI_disStr(Coordinate tl, const char* s, sFONT* font, COLOR bg, COLOR fg);

/**
  * @brief This function displays a number.
	*        If the number does not fit into a line, a newline will be inserted.
  *
  * @param  tl         Top left pixel of the string
  * @param  no         Number to be displayed (including newline)
  * @param  font       STM32 font to be used
  * @param  bg         Background color
  * @param  fg         Foreground color
  * @retval none
  */
extern void GUI_disNum(Coordinate tl, int32_t no,sFONT* font, COLOR bg, COLOR fg);

// Untested function
void GUI_DisGrayMap(POINT Xpoint, POINT Ypoint, const unsigned char *pBmp);

/**
  * @brief  This function writes a sequence of pixels to LCD display.
	*
	* @param  tl     Position of first pixel that should be send to LCD display. 
	* @param  width  Number of pixel that should be written.
	* @param  colors A list of width colors of pixels that will be written.
  * @retval None
  */
extern void GUI_WriteLine(Coordinate tl, LENGTH width, const COLOR *colors);

#endif

// EOF
