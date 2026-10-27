/**
 ******************************************************************************
 * @file    TFT_board.h
 * @author  XXX
 *
 * @date    3. Feb. 2023
 *
 * @brief   Darstellung des TicTacToe Boards auf dem TFT Display des ITS Boards.
 *
 *          TicTacToe benoetigt ein quadratisches Spielbrett mit 3 x 3 Feldern.
 *          Die einzelnen Felder des Spielbretts werden wie folgt mit den Nummern 
 *          0 bis 8 bezeichnet
 *
 *               |     |
 *            0  |  3  |  6
 *               |     |
 *          -----|-----|-----
 *               |     |
 *            1  |  4  |  7
 *               |     |
 *          -----|-----|-----
 *               |     |
 *            2  |  5  |  8
 *               |     |
 *
 ******************************************************************************
 */
 
#ifndef _TFT_BOARD_H
#define _TFT_BOARD_H

/**
 * @brief  Diese Funktion loescht den Bildschirm und zeichnet ein leeres Spielbrett. 
 *         Es besteht aus den vier Linien, die das Spielbrett in 9 Felder unterteilen.
 *         Die Mittelpunkte der 9 Spielfelder werden mit einem 3x3 Pixel Punkt gekennzeichnet.
 *         
 * @retval none
 */
void drawNewBoard(void);


/**
 * @brief  Diese Funktion zeichnet einen gruenen Kreis auf ein Feld des Spielbretts. 
 *
 * @param  pos Nummer des Felds des Spielbretts, auf das der gruene Kreis gezeichnet wird. 
 * @retval none
 */
void drawCycle(int pos);


/**
 * @brief  Diese Funktion zeichnet ein blaues Quadrat auf ein Feld des Spielbretts. 
 *
 * @param  pos Nummer des Felds des Spielbretts, auf das das blaue Quadart gezeichnet wird. 
 * @retval none
 */
void drawQuad(int pos);


/**
 * @brief  Diese Funktion ueberprueft, ob gerade auf ein Feld des Spielfelds gedrueckt wird. 
 *
 * @retval Falls gerade auf ein Feld des Spielbretts gedrueckt wird, die Nummer des Felds.
 *         Ansonsten -1 (z.B. Es wird nicht auf das LCD gedrueckt oder die Koordinate
 *         des aktuellen Druckpunkts kann keinem Feld des Spielbretts eindeutig zugeordnet
 *         werden.)
 */
int getTouch(void);

#endif
// EOF
