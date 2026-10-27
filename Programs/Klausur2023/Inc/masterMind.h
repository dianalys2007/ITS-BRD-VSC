#ifndef _MASTERMIND_H
#define _MASTERMIND_H

/**
 ******************************************************************************
 * @file    masterMind.h
 * @author  Franz Korf
 *          HAW-Hamburg
 *          Labor fuer technische Informatik
 *          Berliner Tor  7
 *          D-20099 Hamburg
 * @version 1.0
 *
 * @date    11. Jan. 2024
 * @brief   Interface des masterMind Moduls der Mastermind Klausuraufgabe.
 ******************************************************************************
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/**
  * @brief Diese Funktion gibt in einer Zeile des KeyPad Terminals eine Zahl aus.
  *        Diese Zahl ist entweder die gesuchte Zahl oder die aktuell geratene Zahl.
  *        Bei der Ausgabe einer geratenen Zahl werden die Ziffern entsprechend
  *        eingefaerbt.
  *        Der Ausgabe wird ein String vorangestellt. Die Ausgabe endet mit 
  *        einem NewLine.
  * 
  * @param s String, der vor der Zahl ausgegeben wird
  *
  * @param size Groesse der Felder sno und guess
  *
  * @param sno Feld, das die Ziffern der gesuchten Zahl enthaelt
  *  
  * @param guess Feld, das die Ziffern der aktuell geratenen Zahl enthaelt
  *        Ist guess == NULL, so wird die gesuchte Zahl ausgegeben.
  *        Ist guess != NULL, wird die geratene Zahl ausgegeben.
  */
void printNumber(char *s, int size, uint8_t *sno, uint8_t *guess);

/**
  * @brief Diese Funktion ueberprueft, ob guess und sno uebereinstimmen.
  *        In diesem Fall ist das Spiel erfolgreich beendet.
  * 
  * @param size Groesse der Felder sno und guess
  *
  * @param sno Feld, das die Ziffern der gesuchten Zahl enthaelt
  *  
  * @param guess Feld, das die Ziffern der aktuell geratenen Zahl enthaelt
  *
  * @retval true: sno und guess stimmen ueberein
  */
bool won(int size, uint8_t *sno, uint8_t *guess);

/**
  * @brief Diese Funktion fuellt sno mit einer neuen Zufallszahl.
  *
  * @param size Groesse des Felds sno
  *
  * @param sno Feld, das mit der neuen Zufallszahl gefuellt wird
  *  
  */
void genNewNumber(int size, uint8_t *sno);

#endif
// EOF
