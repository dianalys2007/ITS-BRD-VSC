/**
 ******************************************************************************
 * @file    io.c
 *
 * @date    25. Juli 2024
 * @brief   Template der io.c für die Laborklausur GS SoSe 2024
 *
 * @author  
 *
 ******************************************************************************
 */

#include "main.h"
#include "stm32f429xx.h"

bool getButton(int n){
    if ((GPIOF -> IDR & (1U << n)) == 0) {
        return true;
    }
    return false;
}

void setLEDs(bool* leds){

    for (int i = 0; i < 8; i++) {
        if (leds[i] == true) {
           GPIOD->BSRR = (1U << i); 
        }
    }
}

// EOF