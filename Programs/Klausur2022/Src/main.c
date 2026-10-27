/**
 ******************************************************************************
 * @file    main.c
 * @author  TI Team
 *          HAW-Hamburg
 *          Labor für technische Informatik
 *          Berliner Tor  7
 *          D-20099 Hamburg
 * @version 1.1
 *
 * @date    17. Juli 2020
 * @brief   Rahmen fuer C Programme, die auf dem ITSboard ausgefuehrt werden.
 *
 *					25.04.2022 Tobias De Gasperis: printf hinzugefuegt
 ******************************************************************************
 */

#include <stdio.h>
#include "io.h"
#include "stm32f4xx_hal.h"
#include "init.h"
#include "delay.h"
#include "LCD_GUI.h"
#include "LCD_Demos.h"
#include "lcd.h"
#include "fontsFLASH.h"
#include "LCD_Touch.h"
#include "error.h"
#include "test.h"
#include "TFT_board.h"

static int getWinner(int board[9])
{
    // drei Reihen
    if (board[0] != 0 && board[0] == board[1] && board[1] == board[2])
        return board[0];

    if (board[3] != 0 && board[3] == board[4] && board[4] == board[5])
        return board[3];

    if (board[6] != 0 && board[6] == board[7] && board[7] == board[8])
        return board[6];

    // drei Spalten
    if (board[0] != 0 && board[0] == board[3] && board[3] == board[6])
        return board[0];

    if (board[1] != 0 && board[1] == board[4] && board[4] == board[7])
        return board[1];

    if (board[2] != 0 && board[2] == board[5] && board[5] == board[8])
        return board[2];

    // zwei Diagonalen
    if (board[0] != 0 && board[0] == board[4] && board[4] == board[8])
        return board[0];

    if (board[2] != 0 && board[2] == board[4] && board[4] == board[6])
        return board[2];

    return 0;
}	

static bool isBoardFull(int board[9])
{
	for (int i = 0; i < 9; i++) {
		if (board[i] == 0) {
			return false;
		}
	}
	return true;
}

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void){

	int board[9] = {0};
	int currentPlayer = 1;
	bool gameOver = false;

	initITSboard();                 // Initialisierung des ITS Boards
	GUI_init(DEFAULT_BRIGHTNESS);   // Initialisierung des LCD Boards mit Touch
	TP_Init(false);                 // Initialisierung des LCD Boards mit Touch
	if (!checkVersionFlashFonts()) {
	    // Ueberpruefe Version der Fonts im Flash passt nicht zur Software Version
		Error_Handler();
	}
	

    //Zum testen in der Datei test.h die passende Aufgabe definieren
	//testIO();
	//testBoard();
    //testTouch();
	
	drawNewBoard();

	

	while (1) { 
		

		if (buttonPressed(0)) {
			for (int i = 0; i < 9; i++) {
				board[i] = 0;
			}

			currentPlayer = 1;
			gameOver = false;

			drawNewBoard();

			switchLEDsOff(0xFFFF);
			switchLEDsOn(0xF000);
		}

		if (gameOver) {
			continue;
		}
		int pos = getTouch();
		

		if(pos != -1 && board[pos] == 0)
		{
			if (currentPlayer == 1) {
				drawCycle(pos);
				board[pos] = 1;
				currentPlayer = 2;
				switchLEDsOff(0xF000);
				switchLEDsOn(0x000F);
			}else if (currentPlayer == 2) {
				drawQuad(pos);
				board[pos] = 2;
				currentPlayer = 1;
				switchLEDsOff(0x000F);
				switchLEDsOn(0xF000);
			}
		}

		int winner = getWinner(board);

		if(winner == 1)
		{
			switchLEDsOff(0xFFFF);
			switchLEDsOn(0xFF00);
			gameOver = true;
		}else if (winner == 2) {
			switchLEDsOff(0xFFFF);
			switchLEDsOn(0x00FF);
			gameOver = true;
		}else if (winner == 0 && isBoardFull(board)) {
			gameOver = true;
		}
		
		

	}
}

// EOF
