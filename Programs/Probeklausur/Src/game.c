/**
 ******************************************************************************
 * @file    game.c
 *
 * @date    25. Juli 2024
 * @brief   Template der game.c f�r die Laborklausur GS SoSe 2024
 *
 * @author
 *
 ******************************************************************************
 */

#include "main.h"
#include "display.h"
#include "io.h"
#include "touch.h"


extern piece_t pieces[9];

#ifdef AUFGABE6

#define ABSOLUTE(A) ((A) < 0 ? ((A) * -1) : A)

bool isNeighbour(int pos0, int pos1) {
	// pos0 or pos1 out of range
	if (pos0 < 0 || BLOCK_COUNT <= pos0 || pos1 < 0 || BLOCK_COUNT <= pos1) {
		return false;
	}

	// divide pos into x and y (same to piecePosition)
	int pos0X = pos0 % 3;
	int pos0Y = pos0 / 3;
	int pos1X = pos1 % 3;
	int pos1Y = pos1 / 3;
	
	// neigbours are exactly one apart in Column with same Rwo or in Row with same Column.
	// So the absolute difference is equal to 1.
	// 0 would be the same and 2 would be one block in between.	
	return (pos0X == pos1X && ABSOLUTE(pos0Y - pos1Y) == 1)
		|| (pos0Y == pos1Y && ABSOLUTE(pos0X - pos1X) == 1);
}

void getPosLED(bool* leds) {
	for (int i = 0; i < BLOCK_COUNT; i++) {
		// number of piece - 1, because it starts at 1
		// new number of -1 is the space
		int numberMinus1 = pieces[i].number - 1;
		// if not space
		if (numberMinus1 != -1) {
			// set led at number Index (with -1) if the number (wihtout -1) matches i
			leds[numberMinus1] = pieces[i].number == i;
		}
	}
}

bool checkGameOver(void) {
	// use led method for game over
	bool leds[LED_COUNT];
	getPosLED(leds);
	// all leds must be on for a win. If one led is off, it's not over.
	bool allLedsOn = true;
	for (uint8_t i = 0; i < LED_COUNT; i++) {
		if (leds[i] == 0) {
			allLedsOn = false;
		}
	}
	return allLedsOn;
}

/**
 * @brief Returns the index of the empty block in pieces array
 * 
 * @retval index of pieces array
 */
int findEmptyBlock() {
	for (int i = 0; i < BLOCK_COUNT; i++) {
		if (pieces[i].number == 0) {
			return i;
		}
	}
	// should not happen
	return -1;
}

/**
 * @brief Gets the LEDs values and sets the leds
 */
void updateLEDs() {
	bool leds[LED_COUNT];
	getPosLED(leds);
	setLEDs(leds);
}

void startGame(void){
	// one run of this method represents one game
	// is called in main method in a loop

	// start game
	initPuzzle();
	drawPuzzle();
	updateLEDs();

	// wait for S0 released to prevent initialize loop
	while (getButton(0)) {
		// nothing
	}

	// game loop if reset is not pressed
	while (!getButton(0)) {
		int pressedPos = getTouchedBlock();

		// if nothing is pressed, wait for touch or reset
		if (pressedPos == -1) {
			// continue is used for less code indentions
			continue;
		}

		int emptyBlockPos = findEmptyBlock();
		if (emptyBlockPos == -1) {
			// restart game because of error
			// should not happen
			return;
		}

		// if block is not a neighbour of empty block, ignore touch
		if (!isNeighbour(emptyBlockPos, pressedPos)) {
			continue;
		}

		// update pieces array with new numbers
		// empty block is now the pressed number
		pieces[emptyBlockPos].number = pieces[pressedPos].number;
		// pressed number is now the empty block
		pieces[pressedPos].number = 0;

		// update display and leds
		drawPuzzle();
		updateLEDs();

		// check if game is over
		if (checkGameOver()) {
			// wait for S0 for a new game
			while (!getButton(0)) {
				// nothing
			}
			// new exit this method for a new call from main
			return;
		}
	}
}

#endif
// EOF
