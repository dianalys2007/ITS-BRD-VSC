#ifndef MAIN_H
#define MAIN_H

#define AUFGABE3
#define AUFGABE4
#define AUFGABE5
#define AUFGABE6


#define BLOCK_COUNT 9
#define LED_COUNT 8



#include "stm32f4xx.h"
#include "init.h"
#include "stdbool.h"
#include "lcd.h"
#include "LCD_GUI.h"
#include "LCD_Touch.h"
#include "error.h"
#include "fonts.h"
#include "fontsFLASH.h"

typedef struct {
	int number;
	Coordinate tl;
	Coordinate br;
} piece_t;

#endif