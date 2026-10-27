/**
 ******************************************************************************
 * @file    adc.h
 * @author  Silke Behn
 *          HAW-Hamburg
 *          Labor für technische Informatik
 *          Berliner Tor  7
 *          D-20099 Hamburg
 * @version 1.0
 *
 * @date    01.12.2020
 * @brief   Funktionen für den Zugriff auf den ADC
 *****
*/

#ifndef _ADC_H
#define _ADC_H


void initADC(int adc);
int readADCValue(int adc );
void ADC_RegChannelConfig(int adc, int channel);


#endif
