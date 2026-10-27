/**
 ******************************************************************************
 * @file    touch.h
 *
 * @date    22. Juli 2024
 * @brief   Funktionsdeklarationen der touch.h für die Laborklausur GS SoSe 2024
 *
 * @author  Jochen Rust, Silke Behn
 *
 ******************************************************************************
 */

#ifndef __TOUCH_H
#define __TOUCH_H

#include "main.h"

/**
 * @brief  Diese Funktion liefert die Block Position in der die Koordinate c liegt.
 *         
 * @retval bool
 */
int getBlockPos(Coordinate c);

/**
 * @brief Diese Funktion fragt das Touch Pad ab und liefert die Positon zurück, 
 *        welcher Block berührt wurde.
 *         
 * @retval none
 */
int getTouchedBlock(void);

#endif

// EOF
