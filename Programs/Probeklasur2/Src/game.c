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

#include "display.h"
#include "main.h"
#include <stdlib.h>
#include "touch.h"
#include "io.h"

extern piece_t pieces[9];

bool isNeighbour(int pos0, int pos1) {
    if (abs(pos0 - pos1) == 3) {
        return true;
    }else if ((abs(pos0 - pos1) == 1) && ((pos0 / 3) == (pos1/3)) ) {
        return true;
    }
    return false;
}

void getPosLED(bool* leds) {
    for (int i = 0; i < 8; i++) {
        if (pieces[i].number == i) {
            leds[i] = true;
        }else {
            leds[i] = false;
        }
    }
}


bool checkGameOver(void) {
    for (int i = 0; i < 9; i++) {
        if (pieces[i].number != i) {
            return false;
        }
    }

    return true;
}
	
void startGame(void){
    while (!checkGameOver()) {
        if (getButton(0)) {
            return;
        }
        int touchedBlock = getTouchedBlock();
        int emptyPos;
        int temp;

        for (int i = 0; i < 9; i++) {
            if (pieces[i].number == 0) {
                emptyPos = i;
                break;
            }
        }

        if (touchedBlock != -1 && isNeighbour(touchedBlock, emptyPos)) {
            temp = pieces[touchedBlock].number;
            pieces[touchedBlock].number = pieces[emptyPos].number;
            pieces[emptyPos].number = temp;

            drawPuzzle();

            bool leds[8];
            getPosLED(leds);
            setLEDs(leds);
        }

    }
}

// EOF