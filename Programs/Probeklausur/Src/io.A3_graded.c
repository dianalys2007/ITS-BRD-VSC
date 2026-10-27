/**
 ******************************************************************************
 * @file    io.c
 *
 * @date    25. Juli 2024
 * @brief   Template der io.c für die Laborklausur GS SoSe 2024
 *
 * @author
 *
 * 37/40 Pts
 *
 * @remarks 
 *		- getButton(int n): GPIOF-IDR Zugriff im return statement
 *
 ******************************************************************************
 */

#include "main.h"


#ifdef AUFGABE3 
bool getButton(int n){
	// bound check
	if (0 <= n && n <= 7) {
		// get all button values and extract relevant bit
		return (GPIOF->IDR & (1 << n)) == 0;
	}
	// false if out of bounds
	return false;
}


void setLEDs(bool* leds){
	uint32_t onMask = 0;
	uint32_t offMask = 0;

	// set bit on onMask or offMask based on bool value
	for (int i = 0; i < 8; i++) {
		if (leds[i]) {
			onMask |= 1 << i;
		} else {
			offMask |= 1 << i;
		}
	}

	// move mask to correct position and combine
	GPIOD->BSRR = onMask | (offMask << 16);
}

#endif

// EOF
