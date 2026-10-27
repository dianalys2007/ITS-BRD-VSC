/**
 ******************************************************************************
 * @file    test.c
 * @author  Franz Korf
 *
 * @date    15. Jan. 2024
 *
 * @brief  Testfunktionen zur Ueberpruefung der einzelnen Aufgaben.
 *
 ******************************************************************************
 */
 
#include "test.h"
#include "delay.h"
#include <stdio.h>
#include <stdint.h>

#define DELAY    500

#define TEST_SIZE  4

#ifdef AUFGABE3
#include "io.h"

void testIO(void){
   // Schalte 0 bis 16 LEDs der Reihe nach an
   for(int i = 0; i <= 16; i++){
      setLEDs(i);
      delay(DELAY);
   }
   
   // Schalte alle LEDs aus
   setLEDs(0);
   delay(DELAY); 
   
   // Schalte 10 LEDs an
   setLEDs(10);
   delay(DELAY); 
   
   // Schalte 3 LEDs an
   setLEDs(3);
   delay(DELAY);
   while(1) {}
}
#endif

#ifdef AUFGABE4
#include "display.h"

/**
  * @brief Diese Funktion fuehrt einen Test der nextInput Funktion aus.
  *
  * @param startText Dieser Text wird vorab ausgegeben.
  * 
  * @param resetOnly true: Es darf nur eine Reset-Key eingegeben werden.
  * 
  * @param retValue Erwarteter return Wert von nextInput.
  * 
  * @param stelle1000 Erwarteter Wert der 10^3 Stelle
  * 
  * @param stelle100 Erwarteter Wert der 10^2 Stelle
  * 
  * @param stelle10 Erwarteter Wert der 10^1 Stelle
  * 
  * @param stelle1 Erwarteter Wert der 10^0 Stelle
  */
static void oneDisplayTest(char *startText, bool resetOnly, char retValue,
   uint8_t stelle1000, uint8_t stelle100, uint8_t stelle10, uint8_t stelle1) {
   uint8_t no[TEST_SIZE];
   printStr(startText, DEFAULT_COLOR);
   for (int i = 0; i < TEST_SIZE; i++){
      no[i] = 0;
   } 
   char erg = nextInput(resetOnly, TEST_SIZE, no);
   
   if (resetOnly && (erg != RESET_KEY) && (erg != RESET_AND_HELP_KEY)){
      printStr("nextInput Error: RESET_KEY oder RESET_AND_HELP_KEY erwartet\n", MISS_COLOR);
      delay(2*DELAY);
      return;
   }
   if (resetOnly && ((RESET_KEY == erg) || (RESET_AND_HELP_KEY == erg)) && (erg == retValue)){
      printStr("nextInput: Test o.k.\n", HIT_COLOR);
      return;
   }
   // es gilt: resetOnly == false
   if (erg != retValue){   
      printStr("nextInput Error: falscher return Wert\n", MISS_COLOR);
      delay(2*DELAY);
      return;
   }
   if (NEXT_GUESS_READ == erg){
      if ((no[0] != stelle1000) || (no[1] != stelle100) || (no[2] != stelle10) || (no[3] != stelle1)){   
         printStr("nextInput Error: Falsche Zahl eingelesen\n", MISS_COLOR);
         delay(2*DELAY);
         return;
      }
   }
   printStr("nextInput: Test o.k.\n", HIT_COLOR);
   return;
}

void testDisplay(void){
   initDisplay();
   // Auf dem Display wird angezeigt, welche Taste des KeyPad gedrueckt werden soll.
   oneDisplayTest("Druecke die Tasten 1 2 7 8 ' '\n", false, NEXT_GUESS_READ, 1, 2, 7, 8);
   delay(DELAY);
   clearDisplay();

   oneDisplayTest("Druecke die Tasten - 7 8 + 3 4 ' '\n", false, NEXT_GUESS_READ, 7, 8, 3, 4);
   delay(DELAY);
   clearDisplay();
   
   oneDisplayTest("Druecke die Tasten + - 7 8 3 4 C\n", true, RESET_KEY, 0, 0, 0, 0);
   delay(DELAY); 
   clearDisplay();   

   oneDisplayTest("Druecke die Taste *\n", false, RESET_AND_HELP_KEY, 0, 0, 0, 0);
   delay(DELAY);
   clearDisplay();   
   
   oneDisplayTest("Druecke die Tasten 5 6 C\n", false, RESET_KEY, 0, 0, 0, 0);
   delay(DELAY);
   clearDisplay();
   
   oneDisplayTest("Druecke die Tasten 5 6 5 7 d # 4 5 6 C\n", false, RESET_KEY, 5, 6, 5, 7);
   while(1) {}
}
#endif

#ifdef AUFGABE5 
#include "masterMind.h"
#include "display.h"

void testMasterMind(void){
   uint8_t sno[TEST_SIZE];
   uint8_t guess[TEST_SIZE];
   initDisplay();
   printStr("Test Funktion won\n", DEFAULT_COLOR);
   for(int i = 0; i < TEST_SIZE; i++){
      sno[i] = i;
      guess[i] = i;
   }
   if (won(TEST_SIZE, sno, guess)) {
      printStr("won: Erster Test erfolgreich.\n", HIT_COLOR);
   } else {
      printStr("won: Erster Test fehlgeschlagen.\n", MISS_COLOR);
   }
   delay(DELAY); 
   guess[2] = 9;
   if (!won(TEST_SIZE, sno, guess)) {
      printStr("won: Zweiter Test erfolgreich.\n", HIT_COLOR);
   } else {
      printStr("won: Zweiter Test fehlgeschlagen.\n", MISS_COLOR);
   }
   delay(5*DELAY); 
   clearDisplay();
   
   printStr("Test Funktion genNewNumber\n", DEFAULT_COLOR);
   // Erzeuge mehrere Zufallszahlen hintereinander. In mehreren
   // Versuchen sollte mindestens eine Abweichung auftreten.
   genNewNumber(TEST_SIZE, sno);
   bool erg = true;
   for (int i = 0; i < 5; i++){
       genNewNumber(TEST_SIZE, guess);
       erg = erg && won(TEST_SIZE, sno, guess);
   }
   if (!erg) {
      printStr("genNewNumber: Test erfolgreich.\n", HIT_COLOR);
   } else {
       printStr("genNewNumber: Test fehlgeschlagen.\n", MISS_COLOR);
   }
   delay(5*DELAY);
   clearDisplay();
      
   printStr("Test Funktion printNumber\n", DEFAULT_COLOR); 
   // Die erwartete Ausgabe wird auf dem Terminal ausgegeben.   
   for(int i = 0; i < TEST_SIZE; i++){
      sno[i] = i + 1;
      guess[i] = i + 1;
   }
   printNumber("1234 in blau :", TEST_SIZE, sno, guess);  
   delay(DELAY);
 
   printNumber("1234 in schwarz :", TEST_SIZE, sno, NULL);
   delay(DELAY);

   for(int i = 0; i < TEST_SIZE; i++){
      guess[i] = guess[i] - 1;
   }   
   printNumber("0123 in rot :", TEST_SIZE, sno, guess);
   delay(DELAY);     
    
   for(int i = 0; i < TEST_SIZE; i++){
      sno[i] = i + 3;
      guess[i] = i  + 3;
   }
   guess[0] = 7;
   guess[TEST_SIZE - 1] = 3;
   printNumber("7 in rot, 4 in blau,  5 in blau, 3 in rot: ", TEST_SIZE, sno, guess);
   
   while(1) {}
}
#endif
// EOF
