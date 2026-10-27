/**
  ******************************************************************************
  * @file    main.c
  * @author  Franz Korf
  * @brief   Kleines Testprogramm fuer neu erstelle Fonts.
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/


#include "LCD_GUI.h"
#include "init.h"
#include "input.h"
#include "bmp.h"
#include "perfTimer.h"
#include "stm32f4xx_hal.h"


int main(void) {
	initITSboard();
	GUI_init(DEFAULT_BRIGHTNESS);
	//GUI_clear(RED);
	initInput();
	initPerfTimers();


	while (1) {
	
		openNextFile();
		//GUI_clear(GREEN);
		

		readHeaders();
		checkHeaders();

		//GUI_clear(RED);
		drawBMP24();
		//GUI_clear(BLACK);
		HAL_Delay(1000);	
	}
}

