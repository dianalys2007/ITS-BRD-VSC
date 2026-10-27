/**
 ******************************************************************************
 * @file    io.h
 *
 * @date    22. Juli 2024
 * @brief   Funktionsdeklarationen der io.h für die Laborklausur GS SoSe 2024
 *
 * @author  Jochen Rust
 *
 ******************************************************************************
 */

#ifndef __IO_H
#define __IO_H

#include "main.h"

/**
 * @brief Diese Funktion zeigt an, ob Taster n (0 <= n <= 7) gedrückt (true) oder
 *        nicht gedrückt (false) ist und liefert diesen Wert über return zurück
 *        true als bool zurück
 *         
 * @retval bool
 */
bool getButton(int n);


/**
 * @brief  Diese Funktion steuert die Outputs OUT0 bis OUT7 und somit die LEDs 
 *         D8 bis D15. bool* leds ist ein Array mit (maximal) 8 Einträgen.
 *         Ist led[i] == true, dann wird OUT<i> auf high gesetzt. Ist led[i] == false, 
 *         dann wird OUT<i> auf low gesetzt.
 *         
 * @retval none
 */
void setLEDs(bool* leds);

#endif

// EOF