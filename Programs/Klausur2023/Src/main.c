/**
 ******************************************************************************
 * @file    main.c
 * @author  TI Team
 *          HAW-Hamburg
 *          Labor fuer technische Informatik
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
 
#include "io.h"
#include "masterMind.h"
#include "stm32f4xx_hal.h"
#include "init.h"
#include "delay.h"
#include "LCD_GUI.h"
#include "LCD_Demos.h"
#include "lcd.h"
#include "fontsFLASH.h"
#include "LCD_Touch.h"
#include "error.h"

#include <stdlib.h>
#include <stdint.h>
#include "test.h"
#include "display.h"

#define MAX_TRYS               16   // Maximale Anzahl an Versuchen
#define NO_DIGITS               4   // Anzahl Stellen der gesuchten Zahl

// Die gesuchte und die akt. geratene Zahl werden als ein Feld von ASCII Zeichen dargestellt.
static uint8_t sno[NO_DIGITS];         // Die gesuchte Zahl
static uint8_t guess[NO_DIGITS];       // Die akt. geratene Zahl


int main(void)
{
    initITSboard();
    initDisplay();

    if (!checkVersionFlashFonts()) {
        Error_Handler();
    }

    int tries;
    bool showHelp = false;

    while (1) {
        /* Neues Spiel initialisieren */
        clearDisplay();
        genNewNumber(NO_DIGITS, sno);
        setLEDs(0);
        tries = 0;

        /* Nach einem Neustart mit '*' die neue Zahl anzeigen */
        if (showHelp) {
            printNumber("Secret: ", NO_DIGITS, sno, sno);
            printStr("\n", DEFAULT_COLOR);
        }

        showHelp = false;

        while (tries < 16) {
            printStr("Enter number: ", DEFAULT_COLOR);

            char input = nextInput(false, NO_DIGITS, guess);
            printStr("\n", DEFAULT_COLOR);

            /* Laufendes Spiel mit C abbrechen */
            if (input == RESET_KEY) {
                break;
            }

            /* Laufendes Spiel mit * abbrechen.
               Das folgende neue Spiel zeigt die neue Geheimzahl. */
            if (input == RESET_AND_HELP_KEY) {
                showHelp = true;
                break;
            }

            /* Vollständiger Versuch wurde eingegeben */
            printNumber("Guess: ", NO_DIGITS, sno, guess);
            printStr("\n", DEFAULT_COLOR);

            tries++;
            setLEDs(tries);

            /* Gewinn prüfen */
            if (won(NO_DIGITS, sno, guess)) {
                printStr("You won!\n", DEFAULT_COLOR);
                printStr("Press C or * to restart\n", DEFAULT_COLOR);

                char restart = nextInput(true, NO_DIGITS, guess);

                if (restart == RESET_AND_HELP_KEY) {
                    showHelp = true;
                }

                break;
            }

            /* Nach dem 16. falschen Versuch verloren */
            if (tries == 16) {
                printStr("You lost!\n", DEFAULT_COLOR);
                printStr("Press C or * to restart\n", DEFAULT_COLOR);

                char restart = nextInput(true, NO_DIGITS, guess);

                if (restart == RESET_AND_HELP_KEY) {
                    showHelp = true;
                }

                break;
            }
        }
    }
}