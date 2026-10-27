/**
 ******************************************************************************
 * @file    display.c
 * @author  Franz Korf
 *          HAW-Hamburg
 *          Labor fuer technische Informatik
 *          Berliner Tor  7
 *          D-20099 Hamburg
 * @version 1.0
 *
 * @date    22. Jan. 2025
 * @brief   Dieses Modul implementiert die LCD Ausgabe der W�rfel.
 ******************************************************************************
 */

#include "display.h"
#include "main.h"
#include "LCD_GUI.h"
#include "error.h"

// Dieses Feld speichert die Positionen der W�rfel. Die Position eines W�rfels
// ist durch seine linke obere und rechte untere Ecke beschrieben.

const struct {
	 Coordinate tl;
	 Coordinate br;
	} dicePosData[NO_DICE] = { {.tl = {.x = 382, .y = 40}, .br = {.x = 465, .y = 123}},
		                         {.tl = {.x = 292, .y = 40}, .br = {.x = 375, .y = 123}},
														 {.tl = {.x = 202, .y = 40}, .br = {.x = 285, .y = 123}},
														 {.tl = {.x = 112, .y = 40}, .br = {.x = 195, .y = 123}},
														 {.tl = {.x = 22,  .y = 40}, .br = {.x = 105, .y = 123}}};

// Von der linken oberen Ecke eines W�rfels ausgehend, beschreiben folgende Konstanten 
// den Offset der Mittelpunkte der einzelnen Positionen der W�rfelaugen / W�rfelpunkte.
//
//           |-------------------------|
//           |                         |
//           |  Pos1             Pos2  |
//           |                         |
//           |  Pos3    Pos4     Pos5  |
//           |                         |
//           |  Pos6             Pos7  |
//           |                         |
//           |-------------------------|
											
const Coordinate offsetPos1 = {.x = 1 * 21, .y = 1 * 21};
const Coordinate offsetPos2 = {.x = 3 * 21, .y = 1 * 21};
const Coordinate offsetPos3 = {.x = 1 * 21, .y = 2 * 21};
const Coordinate offsetPos4 = {.x = 2 * 21, .y = 2 * 21};
const Coordinate offsetPos5 = {.x = 3 * 21, .y = 2 * 21};
const Coordinate offsetPos6 = {.x = 1 * 21, .y = 3 * 21};
const Coordinate offsetPos7 = {.x = 3 * 21, .y = 3 * 21};

#define RADIUS_DICE_EYE       6   // 

void clearDisplay(void)
{
	GUI_clear(GREEN);
}

static void drawEye(uint8_t die, Coordinate offset)
{
	Coordinate center;

	center.x = dicePosData[die].tl.x + offset.x;
	center.y = dicePosData[die].tl.y + offset.y;

	GUI_drawCircle(center, RADIUS_DICE_EYE, BLACK, true, DOT_PIXEL_1X1);
}

void displayDie(uint8_t die, uint8_t value, COLOR c)
{
	GUI_drawRectangle(dicePosData[die].tl, dicePosData[die].br, c, true, DOT_PIXEL_1X1);

	switch(value)
	{
		case 1:
			drawEye(die, offsetPos4);
			break;
		case 2:
			drawEye(die, offsetPos2);
			drawEye(die, offsetPos6);
			break;
		case 3:
			drawEye(die, offsetPos2);
			drawEye(die, offsetPos4);
			drawEye(die, offsetPos6);
			break;
		case 4:
			drawEye(die, offsetPos1);
			drawEye(die, offsetPos2);
			drawEye(die, offsetPos6);
			drawEye(die, offsetPos7);
			break;
		case 5:
			drawEye(die, offsetPos1);
			drawEye(die, offsetPos2);
			drawEye(die, offsetPos4);
			drawEye(die, offsetPos6);
			drawEye(die, offsetPos7);
			break;
		case 6:
			drawEye(die, offsetPos1);
			drawEye(die, offsetPos2);
			drawEye(die, offsetPos3);
			drawEye(die, offsetPos5);
			drawEye(die, offsetPos6);
			drawEye(die, offsetPos7);
			break;


	}
}




// EOF