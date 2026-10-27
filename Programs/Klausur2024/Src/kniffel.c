/**
 ******************************************************************************
 * @file    kniffel.c
 * @author  Franz Korf
 *          HAW-Hamburg
 *          Labor fuer technische Informatik
 *          Berliner Tor  7
 *          D-20099 Hamburg
 * @version 1.1
 *
 * @date    20. Jan. 2025
 * @brief   Dieses Modul speichert den Zustand der W�rfel.
 *
 ******************************************************************************
 */

#include "kniffel.h"
#include "error.h"
#include "main.h"
#include <stdbool.h>
#include <stdlib.h>
#include "random.h"

static uint8_t dice[5];
static bool fixed[5];

uint8_t getDie(uint8_t die)
{
    return dice[die];
}

bool isKniffel(void)
{
    for (int i = 0; i < 5; i++) {
        if (getDie(i) != getDie(0)) {
            return false;
        }
    }
    return true;
}

bool isStreet(void)
{
    bool vorhanden[7] = {false};

    for (int i = 0; i < 5; i++) {
        vorhanden[getDie(i)] = true;
    }

    if (vorhanden[1] && vorhanden[2] && vorhanden[3] && vorhanden[4] && vorhanden[5]) {
        return true;
    }else if (vorhanden[6] && vorhanden[2] && vorhanden[3] && vorhanden[4] && vorhanden[5]) {
        return true;
    }

    return false;
}

void fixDie(uint8_t die)
{
    if (die >= 5) {
        Error_Handler();
    }
    fixed[die] = true;
}

void releaseDice(void)
{
    for (int i = 0; i < 5; i++) {
        fixed[i] = false;
    }
}

bool isFixed(uint8_t die)
{
    if (die >= 5) {
        Error_Handler();
    }
    return fixed[die];
}

void rollDice(void)
{
    for (int i = 0; i < 5; i++) {
        if (!isFixed(i)) {
            dice[i] = (nxtRandNumber() % 6) + 1; 
        }
    }
}

void releaseDie(uint8_t die)
{
    fixed[die] = false;
}


// EOF