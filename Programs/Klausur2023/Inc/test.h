/**
 ******************************************************************************
 * @file    test.h
 * @author  Franz Korf
 *
 * @date    3. Feb. 2023
 *
 * @brief  Interface des test Moduls.
 *
 ******************************************************************************
 */

#ifndef _TEST_H
#define _TEST_H

// #define AUFGABE3
// #define AUFGABE4
// #define AUFGABE5

#ifdef AUFGABE3
/**
 * @brief  Test des Moduls io. 
 */
void testIO(void);
#endif

#ifdef AUFGABE4
/**
 * @brief  Test des Moduls display. 
 */
void testDisplay(void);
#endif

#ifdef AUFGABE5 
/**
 * @brief  Test des Moduls masterMind. 
 */
void testMasterMind(void);
#endif

#endif
// EOF
