/**
 ******************************************************************************
 * @file    touch.c
 * @author  
 * @date    25. Juli 2024
 * @brief   Template der touch.c f�r die Laborklausur GS SoSe 2024
 *
 * @author  
 *
 ******************************************************************************
 */


#include "LCD_Touch.h"
#include "LCD_general.h"
#include "main.h"

extern piece_t pieces[9];

int getBlockPos(Coordinate c){
    for (int i = 0; i < 9; i++) {
        if (c.x >= pieces[i].tl.x && c.x <= pieces[i].br.x && c.y >= pieces[i].tl.y && c.y <= pieces[i].br.y) {
            return i;
        }
    }

    return -1;
}


int getTouchedBlock(void){
    if (TP_pressed()) {
        Coordinate c = TP_get();

        return getBlockPos(c);
    }
    return -1;
}

// EOF
	