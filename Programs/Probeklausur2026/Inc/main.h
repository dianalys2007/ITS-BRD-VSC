/**
 ******************************************************************************
 * @file    main.h
 *
 * @date    22. Juli 2024
 * @brief   globale header-Datei für die Laborklausur GS SoSe 2024
 *
 * @author  Jochen Rust
 *
 ******************************************************************************
 */


#ifndef __MAIN_H
#define __MAIN_H

// std includes
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

// ITS-board includes
#include "stm32f4xx_hal.h"
#include "init.h"
#include "delay.h"
#include "LCD_GUI.h"
#include "LCD_Demos.h"
#include "lcd.h"
#include "fontsFLASH.h"
#include "LCD_Touch.h"
#include "error.h"


// Typ-Deklaration eines Blocks (piece)
typedef struct {
	int number;
	Coordinate tl;
	Coordinate br;
} piece_t;

#endif

// EOF
