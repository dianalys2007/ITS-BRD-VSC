/**
 ******************************************************************************
 * @file    touch.c
 * @author  
 * @date    25. Juli 2024
 * @brief   Template der touch.c f�r die Laborklausur GS SoSe 2024
 *
 * @author
 *
 * 40/40 pts
 *
 ******************************************************************************
 */


#include "main.h"

extern piece_t pieces[BLOCK_COUNT];

#ifdef AUFGABE5

int getBlockPos(Coordinate c){
	// check every block if the block was pressed
	for (int i = 0; i < BLOCK_COUNT; i++) {
		Coordinate tl = pieces[i].tl;
		Coordinate br = pieces[i].br;

		// if the coordinate is between the x value of top-left und bottom-right and the same for y.
		if (tl.x <= c.x && c.x <= br.x && tl.y <= c.y && c.y <= br.y) {
			// this block was pressed -> return pos of current block
			return i;
		}
	}
	// coordinate is not in any block
	return -1;
}


int getTouchedBlock(void){
	// check if pressed or else return -1
	if (!TP_pressed()) {
		return -1;
	}
	// get block position or -1 if no block was pressed
	return getBlockPos(TP_get());
}

#endif

// EOF
