/**
 ******************************************************************************
 * @file    game.h
 *
 * @date    22. Juli 2024
 * @brief   Funktionsdeklarationen der game.h für die Laborklausur GS SoSe 2024
 *
 * @author Silke Behn
 *
 ******************************************************************************
 */

#ifndef __GAME_H
#define __GAME_H

#include "main.h"


/**
 * @brief In dieser Funktion wird überprüft, ob die beiden Blöcke an den Positionen
 *        pos0 und pos1 benachbart sind.
 *         
 * @retval none
 */
bool isNeighbour(int pos0, int pos1);


/**
 * @brief In dieser Funktion wird abgefragt, ob ein Block an der richtigen Stelle 
 *        im Puzzle steht (number==position). In diesem Fall wird Stelle i im 
 *        bool[9]-Array true gesetzt, anderfalls false.
 *         
 * @retval none
 */
void getPosLED(bool* leds);


/**
 * @brief Diese Funktion zeigt an, ob alle Blöcke an der richtigen Stelle stehen, 
 *        und liefert in dem Fall true zurück, andernfalls false.
 *         
 * @retval bool
 */
bool checkGameOver(void);

/**
 * @brief Diese Funktion startet ein Spiel, das vorher initialisiert sein muss
 *         
 * @retval bool
 */
void startGame(void);

#endif

// EOF