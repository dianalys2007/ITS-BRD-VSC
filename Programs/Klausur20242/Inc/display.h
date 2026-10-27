#ifndef _DISPLAY_H
#define _DISPLAY_H
/**
 ******************************************************************************
 * @file    display.h
 * @author  Franz Korf
 *          HAW-Hamburg
 *          Labor fuer technische Informatik
 *          Berliner Tor  7
 *          D-20099 Hamburg
 * @version 1.0
 *
 * @date    22. Jan. 2025
 * @brief   Interface des Moduls zur Ausgabe der Würfel auf dem LCD Display. 
 ******************************************************************************
 */
#include "LCD_GUI.h"

// Farben der Würfel
#define DIE_COLOR_FIXED          MAGENTA
#define DIE_COLOR_WIN            YELLOW
#define DIE_COLOR_DEFAULT        GREEN     

/**
  * @brief Diese Funktion füllt den ganzen Bildschirm mit blau.
  */
void clearDisplay(void);

/**
  * @brief Diese Funktion zeichnet einen Würfel auf dem Bildschirm.
  *
  * @param  die Die Nummer des Würfels, die die Position auf dem Bildschirm 
	*             festlegt. 0 ist der Würfel rechts außen. 4 ist der Würfel
	*             links außen.
	* @param  val Der Wert, der auf dem Würfel dargestellt wird.
  * @param  c   Die Farbe des Würfels.
  */
void displayDie(uint8_t die, uint8_t value, COLOR c);

#endif
// EOF
