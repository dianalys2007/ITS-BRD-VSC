/**
 ******************************************************************************
 * @file    display.c
 * 
 * @date    25. Juli 2024
 * @brief   Template der display.c f�r die Laborklausur GS SoSe 2024
 *
 * @author
 *
 ******************************************************************************
 */

#include "LCD_GUI.h"
#include "LCD_general.h"
#include "main.h"
#include <stdint.h>
#include <stdio.h>
#include "display.h"

#define DIGIT_X 17
#define DIGIT_Y 24
#define SPACER 8
#define OFFSET (6*17)
#define BLOCK_X (SPACER+DIGIT_X*4)
#define BLOCK_Y (SPACER+DIGIT_Y*4)

piece_t pieces[BLOCK_COUNT];

#ifdef AUFGABE4
void initDisplay() {
	GUI_init(DEFAULT_BRIGHTNESS);   // Initialisierung des LCD Boards mit Touch
	TP_Init(false);                 // Initialisierung des LCD Boards mit Touch
	GUI_clear(BLUE);
	lcdSetFont(24);
	if (!checkVersionFlashFonts()) {
	    // Ueberpruefe Version der Fonts im Flash passt nicht zur Software Version
		Error_Handler();
	}

}

void initPuzzle(void) {
	// Initialisierung aller Puzzleteile mit der angezeigten Nummer 
	// darf nicht ver�ndert werden
	pieces[0].number = 1;
	pieces[1].number = 0;
	pieces[2].number = 2;
	pieces[3].number = 3;
	pieces[4].number = 4;
	pieces[5].number = 5;
	pieces[6].number = 6;
	pieces[7].number = 7;
	pieces[8].number = 8;

	// Initialisierung der Position auf dem Spielfeld
	for (int i = 0; i < BLOCK_COUNT; i++) {
		initPiecePosition(i);
	}
}

void initPiecePosition(int pos) {

	if (pos < 0 || pos >= BLOCK_COUNT) {
		return;
	}

	uint8_t xPos = pos % 3;
	uint8_t yPos = pos / 3;

	pieces[pos].tl.x = OFFSET + xPos * BLOCK_X;
	pieces[pos].tl.y = SPACER + yPos * BLOCK_Y;

	pieces[pos].br.x = pieces[pos].tl.x +BLOCK_X - SPACER;
	pieces[pos].br.y = pieces[pos].tl.y +BLOCK_Y - SPACER;
}


void drawPiece(int pos) {
	if (pos < 0 || pos >= BLOCK_COUNT) {
		return;
	}

	if (pieces[pos].number == 0 ) {
		GUI_drawRectangle(pieces[pos].tl, pieces[pos].br, 0x0000, true, DOT_PIXEL_1X1);
	}else {
		GUI_drawRectangle(pieces[pos].tl, pieces[pos].br, 0xFFFF, true, DOT_PIXEL_1X1);
		Coordinate text = pieces[pos].tl;
		text.x += (BLOCK_X - DIGIT_X) / 2;
		text.y += (BLOCK_Y - DIGIT_Y) / 2;

		GUI_disChar(text, '0' + pieces[pos].number, &Font24, 0xFFFF, 0x0000);
	}
}


void drawPuzzle(void) {
	for (int i = 0; i < BLOCK_COUNT; i++) {
		drawPiece(i);
	}
}

#endif

// EOF