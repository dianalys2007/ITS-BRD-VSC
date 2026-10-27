/**
 ******************************************************************************
 * @file    test.c
 * @author  Franz Korf
 *
 * @date    3. Feb. 2023
 *
 * @brief  Testfunktionen zur Ueberpruefung der einzelnen Aufgaben.
 *
 ******************************************************************************
 */
 
#include "test.h"
#include "TFT_board.h"
#include "delay.h"
#include <stdio.h>
#include "lcd.h"

#ifdef AUFGABE3
#include "io.h"
#endif

#define NO_FIELDS          9
#define STD_DELAY          500

#define X  5            // X Position fuer alle Ausgaben
#define Y1 10           // Y Position fuer Ausgabe 1
#define Y2 5            // Y Position fuer Ausgabe 2

#define TXT_SIZE    30
static char txt[TXT_SIZE];

#ifdef AUFGABE3
void testIO(void){
   while (1){
      for(int i = 0; i < 8; i++){
		 lcdGotoXY(X,Y1);
		 snprintf(txt, sizeof(txt), "Bitte Taste %d druecken", i);
		 lcdPrintlnS(txt);
				
         while(!buttonPressed(i)){ };
				switch(i) {
						 case 0: // S0 gedrueckt
							  lcdGotoXY(X,Y2);
							  lcdPrintlnS("Alle LEDs an");
								switchLEDsOn(0xFFFF); // schalte alle LEDs an
								break;
						 case 1: // S1 gedrueckt
							  lcdGotoXY(X,Y2);
							  lcdPrintlnS("Alle geraden LEDs an");
							  switchLEDsOff(0xFFFF);
								switchLEDsOn(0x5555); // schalte die geraden LEDS an, Rest unveraendert
								break;
						 case 2: // S2 gedrueckt
							  lcdGotoXY(X,Y2);
							  lcdPrintlnS("Alle ungeraden LEDs an");
								switchLEDsOff(0x5555); // schalte die geraden LEDs aus
								switchLEDsOn(0xAAAA);  //  und die ungeraden LEDs an
								break;
						 case 3: // S3 gedrueckt
							  lcdGotoXY(X,Y2);
							  lcdPrintlnS("Linken 4 LEDs an, Rest unveraendert");
								switchLEDsOn(0xF000); // schalte die oberen vier LEDs an, Rest unveraendert
								break;
						 case 4: // S4 gedrueckt
							  lcdGotoXY(X,Y2);
							  lcdPrintlnS("Linken 4 LEDs aus, Rest unveraendert");
								switchLEDsOff(0xF000); // schalte die oberen vier LEDs aus, Rest unveraendert
								break;
						 case 5: // S5 gedrueckt
							  lcdGotoXY(X,Y2);
							  lcdPrintlnS("Rechten 4 LEDs an, Rest unveraendert");
								switchLEDsOn(0x000F); // schalte die unteren vier LEDs an, Rest unveraendert
								break;
						 case 6: // S6 gedrueckt
							  lcdGotoXY(X,Y2);
							  lcdPrintlnS("Rechten 4 LEDs aus, Rest unveraendert");
								switchLEDsOff(0x000F); // schalte die unteren vier LEDs aus, Rest unveraendert
								break;
						 case 7: // S7 gedrueckt
							  lcdGotoXY(X,Y2);
							  lcdPrintlnS("Alle LEDs aus");
								switchLEDsOff(0xFFFF); // schalte alle LEDs aus
								break;
					}
         }
      }
}
#endif


#ifdef AUFGABE4 
void testBoard(void){
	int i = 0;
   // Besetze alle 9 Felder abwechseln mit gruenen und blauen
   // Steinen. Sind alle Felder belegt, zeichne ein neues Board.
	while(1){
		int pos = i % NO_FIELDS;
		if (pos == 0) {
			drawNewBoard();
		}
		if (i % 2){
			drawCycle(pos);
		} else {
			drawQuad(pos);
	  }
		delay(STD_DELAY);
		i++;
	}
}
#endif


#ifdef AUFGABE5 
void testTouch(void){
	int i = 0;
	drawNewBoard();
	while(1){
		int pos = -1;
		while (pos == -1){
			 pos = getTouch();
		}
      // Zeichne alternierend Kreise und Quadrate
		if (i % 2){
			drawCycle(pos);
		} else {
			drawQuad(pos);
	  }
		delay(STD_DELAY);
		i++;
      // Zeichne das Spielfeld neu, nachdem 10 Steine gelegt wurden
		if ((i % 10) == 0){
			 drawNewBoard();
			 delay(STD_DELAY/10);
		}
	}
}
#endif




// EOF
