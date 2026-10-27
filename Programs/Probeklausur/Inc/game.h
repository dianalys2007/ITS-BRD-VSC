#ifndef GAME_H
#define GAME_H

#include "main.h"

bool isNeighbour(int pos0, int pos1);

void getPosLED(bool* leds);

bool checkGameOver(void);

int findEmptyBlock();
	
void updateLEDs();

void startGame(void);
	
#endif
