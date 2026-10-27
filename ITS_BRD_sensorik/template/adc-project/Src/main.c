/**
 ******************************************************************************
 * @file    main.c
 * @author  Tobias De Gasperis
 *          HAW-Hamburg
 *          Labor für technische Informatik
 *          Berliner Tor  7
 *          D-20099 Hamburg
 * @version 1.0
 *
 * @date    14. September 2022
 * @brief   Rahmen fuer den Zugriff auf den ADC des Nucleo-F429ZI Boards
 *
 ******************************************************************************
 */

#include <stdio.h>
#include "stm32f4xx_hal.h"
#include "init.h"
#include "delay.h"
#include "LCD_GUI.h"
#include "LCD_Demos.h"
#include "lcd.h"
#include "fontsFLASH.h"
#include "LCD_Touch.h"
#include "error.h"
#include "adc.h"

#define DELAY 200

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void){
	
	int adcValue = 0;
	int measurementNumber = 0;

	initITSboard();                 // Initialisierung des ITS Boards
	GUI_init(DEFAULT_BRIGHTNESS);   // Initialisierung des LCD Boards mit Touch
	TP_Init(false);                 // Initialisierung des LCD Boards mit Touch
	if (!checkVersionFlashFonts()) {
	    // Ueberpruefe Version der Fonts im Flash passt nicht zur Software Version
		Error_Handler();
	}
	
	initADC(3);
	ADC_RegChannelConfig(3,6);

	lcdPrintlnS("Nucleo ADC3 6");
	lcdPrintlnS("");
	lcdPrintlnS("");
	
	for(;;){
		adcValue = readADCValue(3);
		printf("ADC: %d: %d\n", measurementNumber, adcValue);
		HAL_Delay(DELAY);
	}
}

// EOF
