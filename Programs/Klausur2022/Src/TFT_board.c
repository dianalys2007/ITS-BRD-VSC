/**
 ******************************************************************************
 * @file    TFT_board.c
 * @author  XXX
 *
 * @date    3. Feb. 2023
 *
 * @brief   Darstellung des TicTacToe Boards auf dem TFT Display des ITS Boards.
 *
 ******************************************************************************
 */
 
#include "TFT_board.h"
#include "LCD_GUI.h"
#include "LCD_Touch.h"
#include "LCD_general.h"


#define COLOR_BG			GRAY         // Hintergrundfarbe
#define COLOR_LINE		    BLACK        // Farbe der Trennlinien des Spielfelds  
#define COLOR_SP1			GREEN        // Farbe der runden Spielsteine
#define COLOR_SP2			BLUE         // Farbe der quadratischen Spielsteine

#define ZB       5                   // Breite einer Trennlinie
#define QKL      101                 // Kantenlaenge des Quadrats eines Felds des Spielbretts
#define ZL       (3*QKL + 2*ZB)      // Laenge einer Trennlinie des Spielfelds

#define U_X     83
#define U_Y     3

typedef struct {
	Coordinate topLeft;
    Coordinate bottomRight;
} Rectangle;

// Die vier Trennlinien werden als Rechtecke dargestellt. 
// Das folgende Feld enthaelt die Koordinaten dieser Rechtecke.
static Rectangle line[] = {
   {.topLeft     = {.x = U_X,                   .y = U_Y +        QKL},
    .bottomRight = {.x = U_X + ZL,              .y = U_Y +        QKL + ZB}
   },
   {.topLeft     = {.x = U_X,                   .y = U_Y + ZB + 2*QKL},
    .bottomRight = {.x = U_X + ZL,              .y = U_Y + ZB + 2*QKL + ZB} 
   }, 
   {.topLeft     = {.x = U_X + QKL,             .y = U_Y},
    .bottomRight = {.x = U_X + QKL + ZB,        .y = U_Y + ZL} 
   },   
   {.topLeft     = {.x = U_X + ZB + 2*QKL,      .y = U_Y},
    .bottomRight = {.x = U_X + ZB + 2*QKL + ZB, .y = U_Y + ZL} 
   }
};

// Die Mittelpunkte der 9 Felder des Spielbretts
static Coordinate  mp[] = {
    {.x = U_X +                QKL/2 + 1, .y = U_Y +                 QKL/2 + 1},
    {.x = U_X +                QKL/2 + 1, .y = U_Y +   (QKL + ZB) +  QKL/2 + 1},
    {.x = U_X +                QKL/2 + 1, .y = U_Y + 2*(QKL + ZB) +  QKL/2 + 1},

    {.x = U_X +   (QKL + ZB) + QKL/2 + 1, .y = U_Y +                 QKL/2 + 1},
    {.x = U_X +   (QKL + ZB) + QKL/2 + 1, .y = U_Y +   (QKL + ZB) +  QKL/2 + 1},
    {.x = U_X +   (QKL + ZB) + QKL/2 + 1, .y = U_Y + 2*(QKL + ZB) +  QKL/2 + 1},
		
    {.x = U_X + 2*(QKL + ZB) + QKL/2 + 1, .y = U_Y +                 QKL/2 + 1},
    {.x = U_X + 2*(QKL + ZB) + QKL/2 + 1, .y = U_Y +   (QKL + ZB) +  QKL/2 + 1},
    {.x = U_X + 2*(QKL + ZB) + QKL/2 + 1, .y = U_Y + 2*(QKL + ZB) +  QKL/2 + 1}};

void drawNewBoard(void)
{
    GUI_clear(COLOR_BG);
    for (int i = 0; i < 4; i++) {
        GUI_drawRectangle(line[i].topLeft, line[i].bottomRight, COLOR_LINE, true, DOT_PIXEL_1X1);
    }

    for (int i = 0; i < 9; i++) {
        GUI_drawPoint(mp[i], COLOR_LINE, DOT_PIXEL_1X1, DOT_FILL_AROUND);
    }
}

void drawCycle(int pos)
{
    int radius = QKL / 3;

    GUI_drawCircle(mp[pos], radius, COLOR_SP1, true, DOT_PIXEL_1X1);
}

void drawQuad(int pos)
{
    Coordinate tl;
    Coordinate br;
    int halfSide = QKL / 3;

    tl.x = mp[pos].x - halfSide;
    tl.y = mp[pos].y - halfSide;

    br.x = mp[pos].x + halfSide;
    br.y = mp[pos].y + halfSide;

    GUI_drawRectangle(tl, br, COLOR_SP2, true, DOT_PIXEL_1X1);
}

int getTouch(void)
{
    
    if (TP_pressed()) {
       Coordinate c = TP_get();
       for (int i = 0; i < 9; i++) {
        int left = mp[i].x - QKL / 2;
        int right = mp[i].x + QKL / 2;
        int oben = mp[i].y - QKL / 2;
        int unten = mp[i].y + QKL / 2;
        if (c.x >= left && c.x <= right && c.y >= oben && c.y <= unten) {
            return i;
        }
       }
    }
    return -1;
}
// EOF
