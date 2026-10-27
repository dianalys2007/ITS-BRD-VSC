/**
 ******************************************************************************
 * @file    test.h
 * @author  Franz Korf
 *
 * @date    3. Feb. 2023
 *
 * @brief  Testfunktionen zur Ueberpruefung der einzelnen Aufgaben.
 *
 ******************************************************************************
 */

#ifndef _TEST_H
#define _TEST_H


//#define AUFGABE3
//#define AUFGABE4
#define AUFGABE5

/**
 * @brief  Test des Moduls io. 
 *
 * @retval none
 */
#ifdef AUFGABE3
void testIO(void);
#endif

/**
 * @brief  Test der Ausgabefunktionen des Moduls TFT_board. 
 *
 * @retval none
 */
#ifdef AUFGABE4 
   void testBoard(void);
#endif


/**
 * @brief  Test der Touch Funktionalitaet des Moduls TFT_board. 
 *
 * @retval none
 */
#ifdef AUFGABE5 
void testTouch(void);
#endif




#endif
// EOF
