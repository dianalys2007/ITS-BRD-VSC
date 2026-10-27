/**
 ******************************************************************************
 * @file    test.c
 *
 * @date    25. Juli 2024
 * @brief   Testumgebung für die Laborklausur GS SoSe 2024
 *
 * @author  Jochen Rust, Silke Behn
 *
******************************************************************************
 */

#include "main.h"
#include "io.h"
#include "touch.h"

//Beim Drücken der Buttons muss die jeweilige LED Leutchten (INx -> OUTx)
void testButtonsAndLEDs() {
	bool led[8];
	
	while(1) {
		for (int i=0; i<9;i++) {
			led[i] = getButton(i);
		}
		setLEDs(led);
	}
}

//Auf dem Display wird die Position des berührten Blocks ausgegeben.
void testTouch(){
	  int block = 0;
	  
	  while(1) {
			block = getTouchedBlock();
			if ( block >=0 & block <=8){
		     lcdGotoXY(2,5);
             lcdPrintC( block+ '0' );			
			}
		}	
}

// EOF
