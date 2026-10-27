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
 * @date    11. Jan. 2024
 * @brief   Interface des Display Moduls der Mastermind Klausuraufgabe.
 ******************************************************************************
 */
#include "LCD_GUI.h"
#include "terminal.h"
#include <stdint.h>
// Verwendete Textfarben fuer das Terminal
#define DEFAULT_COLOR            BLACK
#define HIT_COLOR                BLUE     // richtig geratene Ziffer
#define MISS_COLOR               RED      // falsch geratene Ziffer

// Return Werte von nextInput
#define RESET_KEY                'C'      // Neustart
#define RESET_AND_HELP_KEY       '*'      // Neustart mit Ausgabe des gesuchten Zahl
#define NEXT_GUESS_READ          ' '      // Zahl wurde eingegeben


/**
  * @brief Diese Funktion initialisiert das LCD Display, die Touch Funktionalitaet
  *        und das KeyPad.
  */
void initDisplay(void);

/**
  * @brief Diese Funktion gibt einen farbigen String auf KeyPad-Terminal aus. 
  *
  * @param  s String, der ausgegeben wird.
  *
  * @param  c Textfarbe des zu druckende Strings.
  */
void printStr(char * s, COLOR c);

/**
  * @brief Diese Funktion loescht das KeyPad-Terminal.
  */
void clearDisplay(void);

/**
  * @brief Diese Funktion liest die nächste Eingabe vom KeyPad.
  *        Sie kann eine genau size Zeichen lange Zahl sein
  *        oder eine der Reset Eingaben (C oder *).
  *        Ein Reset-Key (C oder *) kann an beliebiger Stelle stehen und 
  *        beendet sofort die Eingabe.
  *        Eine Zahl wird mit dem Enter-Key (Key ohne Aufschrift)  
  *        abgeschlossen.
  *        Während des Eingabeprozesses werden alle nicht gueltigen
  *        Zeichen verworfen. Werden mehr als size Ziffern eingegeben,
  *        werden die überzähligen auch verworfen.
  *        Die gültigen eingegebenen Ziffer '0' bis '9' werden auf dem KeyPad-
  *        Terminal ausgegeben.
  *
  * @param resetOnly = true: Als Eingabe wird nur ein Reset-Key akzeptiert.
  *        Dies wird z.B. benoetigt, wenn das Spiel beendet ist und der Spieler
  *        nur einen Reset-Key eingeben darf.
  *  
  * @param size Laenge des Felds a und somit Anzahl der Ziffern, die eingelesen werden.
  *
  * @param a Feld der Laenge size. Die Ziffern (0 bis 9) der eingelesenen
  *        Zahl werden in diesem Feld zuueckgegeben.
  *
  * @retval RESET_KEY, RESET_AND_HELP_KEY oder NEXT_GUESS_READ (s. defines)      
  */
char nextInput(bool resetOnly, int size, uint8_t *a);

#endif
// EOF
