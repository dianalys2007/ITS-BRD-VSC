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
#include "test.h"
#include "display.h"
#include "game.h"
#include "io.h"


int main(void){
	// Initialisierung des ITS Boards
	initITSboard();  

#ifdef AUFGABE4
	// Initialisierung des Touch-Displays
	initDisplay();
#endif

	// Testumgebung für die Abnahme von Aufgabe 3
	//testButtonsAndLEDs();

	// Testumgebung für die Abnahme von Aufgabe 4
	//initPuzzle();
	//drawPuzzle();

	// Testumgebung für die Abnahme von Aufgabe 5
	//initPuzzle();
	//drawPuzzle();
	//testTouch();

	// Testumgebung für die Abnahme von Aufgabe 6
	
	// always call startGame in a loop
	while (true) {
		startGame();
	}
}

// EOF
