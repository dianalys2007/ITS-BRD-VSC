/**
 ******************************************************************************
 * @file    display.h
 *
 * @date    22. Juli 2024
 * @brief   Funktionsdeklarationen der display.h für die Laborklausur GS SoSe 2024
 *
 * @author  Jochen Rust, Silke Behn
 *
 ******************************************************************************
 */

#ifndef __DISPLAY_H
#define __DISPLAY_H

#include "main.h"

/**
 * @brief  Diese Funktion initialisiert das Display inklusive Touch-Funktionalität.
 *         
 * @retval none
 */
void initDisplay(void);


/**
 * @brief  Diese Funktion initialisiert die Ausgabe des Zahlen-Puzzles (9 Blöcke), die 
 *         auf dem Display ausgegeben werden. Die koordinaten des Blocks, die Nummer und 
 *         die Position werden für jeden BLock im Datentyp piece_t gespeichert:
 *         
 * @retval none
 */
void initPuzzle(void);


/**
 * @brief  In dieser Funktion werden jedem Block die Koordinaten auf dem Display zugewiesen. 
 *         
 *         
 * @retval none
 */
void initPiecePosition(int pos);


/**
 * @brief  Diese Funktion ist für die Darstellung eines einzelnen Blocks mit zugehöriger Nummer 
 *         zuständig. Für die Nummern 1-8 wird dabei ein weißes Rechteck gezeichnet. für die 
 *         Lücke (Nummer 0)wird ein schwarzes Rechteck gezeichnet.
 *         
 * @retval none
 */
void drawPiece(int pos);


/**
 * @brief  Mit dieser Funktion wird das gesamte Puzzle auf dem Display ausgegeben.
 *         
 * @retval none
 */
void drawPuzzle(void);


#endif

// EOF