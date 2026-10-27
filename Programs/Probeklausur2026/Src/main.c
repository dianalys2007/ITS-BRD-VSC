/**
 ******************************************************************************
 * @file    main.c
 *
 * @date    25. Juli 2024
 * @brief   Rahmen fuer Klausuraufgabe GS SoSe 2024.
 *
 * @author  
 *
 ******************************************************************************
 */

#include "main.h"
#include "display.h"
#include "stm32f4xx_hal.h"
#include "test.h"
#include "game.h"
#include "io.h"


int main(void){
	// Initialisierung des ITS Boards
	initITSboard();                 
	// Initialisierung des Touch-Displays
	initDisplay();

	// Testumgebung für die Abnahme von Aufgabe 3
	//testButtonsAndLEDs();
	
	// Testumgebung für die Abnahme von Aufgabe 4
	//initPuzzle();
	//drawPuzzle();
	
	// Testumgebung für die Abnahme von Aufgabe 5
	initPuzzle();
	drawPuzzle();
	//testTouch();

	// Testumgebung für die Abnahme von Aufgabe 6
	while (1) {
		initPuzzle();
		drawPuzzle();
		startGame();

		while (!getButton(0)) {
		
		}
	}
}

// EOF
