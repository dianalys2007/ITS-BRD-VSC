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
#include <iso646.h>
#include <stdlib.h>
#include "touch.h"
#include "io.h"

extern piece_t pieces[9];

bool isNeighbour(int pos0, int pos1) {
    if (abs(pos0 - pos1) == 3) {
        return true;
    }else if (((pos0 / 3) == (pos1 / 3)) && abs(pos0 - pos1) == 1) {
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

        int touchedPos = getTouchedBlock();
        int emptyPos = -1;
        int temp;

    
        for (int i = 0; i < 9; i++) {
            if (pieces[i].number == 0) {
                emptyPos = i;
                break;
            }
        }

        if (touchedPos != -1 && isNeighbour(touchedPos,  emptyPos)) {
            temp = pieces[touchedPos].number;
            pieces[touchedPos].number = pieces[emptyPos].number;
            pieces[emptyPos].number = temp;

            drawPuzzle();

            bool leds[8];
            getPosLED(leds);
            setLEDs(leds);
        }
    }
}

// EOF