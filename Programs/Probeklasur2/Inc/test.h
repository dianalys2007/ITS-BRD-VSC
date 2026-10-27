/**
 ******************************************************************************
 * @file    test.h
 *
 * @date    22. Juli 2024
 * @brief   Testumgebung für die Aufgabe 3+5
 *
 * @author  Jochen Rust, Silke Behn
 *
 ******************************************************************************
 */

#ifndef __TEST_H
#define __TEST_H

/**
 * @brief  Diese Funktion testet die richtige Ansteuerung der Taster und LEDs,
 * 		   indem der Tastendruck die jeweilige LED anschaltet
 *         
 * @retval none
 */
void testButtonsAndLEDs();

/**
 * @brief  Diese Funktion testet die richtige Ansteuerung des Touch-Displays,
 * 		   indem die Position des jeweiligen Blocks auf dem Display angezeigt
 *		   wird
 *         
 * @retval none
 */
void testTouch();

#endif

// EOF