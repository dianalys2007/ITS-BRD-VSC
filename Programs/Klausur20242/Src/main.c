/**
 ******************************************************************************
 * @file    main.c
 * @author  Franz Korf
 *          HAW-Hamburg
 *          Labor fuer technische Informatik
 *          Berliner Tor  7
 *          D-20099 Hamburg
 * @version 1.0
 *
 * @date    22. Jan. 2025
 * @brief   Die Klausuraufgabe Kniffel.
 *
 ******************************************************************************
 */

#include "main.h"
#include <stdlib.h>
#include "init.h"
#include "io.h"
#include "display.h"
#include "kniffel.h"
#include "test.h"
#include <stdint.h>
#include "stm32f4xx_hal.h"


int main(void){
   // Initialisierung der HW
   initITSboard();
	 GUI_init(DEFAULT_BRIGHTNESS);
	
#ifdef AUFGABE3
   testIO();
#endif

#ifdef AUFGABE4
   testDisplay();
#endif

#ifdef AUFGABE5 
	 testKniffel();
#endif

   clearDisplay();
   rollDice();
   int count = 0;
   setLEDs(count);

   for (int i = 0; i < 5; i++) {
      displayDie(i, getDie(i), DIE_COLOR_DEFAULT);
   }


   while (1) {
      
      for (int i = 0; i < 5; i++) {
         if (buttonPressed(i)) {
            if (isFixed(i)) {
               releaseDie(i);
               displayDie(i, getDie(i), DIE_COLOR_DEFAULT);
            }else {
               fixDie(i);
               displayDie(i, getDie(i), DIE_COLOR_FIXED);
            }
            
         }
      }

      if (buttonPressed(5)) {
         releaseDice();
         for (int i = 0; i < 5; i++) {
            displayDie(i, getDie(i), DIE_COLOR_DEFAULT);
         }

      }

      if (buttonPressed(6)) {
         rollDice();
         count++;
         setLEDs(count);
         for (int i = 0; i < 5; i++)
         {
            if (!isFixed(i)) {
               displayDie(i, getDie(i), DIE_COLOR_DEFAULT);
            }else {
               displayDie(i, getDie(i), DIE_COLOR_FIXED);
            }
         }
      }

      if(isKniffel() || isStreet())
      {
         for (int i = 0; i < 5; i++) {
            displayDie(i, getDie(i), DIE_COLOR_WIN);
         }
      }

      if (buttonPressed(7)) {
         clearDisplay();
         releaseDice();
         rollDice();
         count = 1;
         setLEDs(count);
         for (int i = 0; i < 5; i++) {
            displayDie(i, getDie(i), DIE_COLOR_DEFAULT);
         }

      }
   }

	
   
}
// EOF