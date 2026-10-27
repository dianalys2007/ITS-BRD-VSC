#ifndef  _RANDOM_H
#define _RANDOM_H

/**
 ******************************************************************************
 * @file    random.h
 * @author  Franz Korf
 *          HAW-Hamburg
 *          Labor fuer technische Informatik
 *          Berliner Tor  7
 *          D-20099 Hamburg
 * @version 1.0
 *
 * @date    11. Jan. 2024
 * @brief   Interface des Standard-Pseudozufallsgenerator fuer die
 *          Mastermind Klausuraufgabe.
 ******************************************************************************
 */

/**
  * @brief Diese Funktion liefert eine Pseudozufallszahl. Der erste Aufruf der
  *        Funktion initialisiert den Zufallszahlengenerator.
  */
int nxtRandNumber(void);

#endif
// EOF
