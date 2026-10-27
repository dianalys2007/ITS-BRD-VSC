#include "timernew.h"
#include "stm32f4xx_hal.h"

uint32_t timer_getTimestamp(void) {//gibt den aktuellen zählerstand des Timers zurück 
    return TIM2->CNT;
}

double timer_getDeltaSeconds(uint32_t t1, uint32_t t2) {//die vergangene zeit zwischen zwei timerwerten
    //t1=früherer timerstamp, t2=aktueller
    uint32_t diff = t2 - t1; // Berechnung der verstrichenen Ticks
    return (double)diff / 90000000.0; // Umrechnung in mSekunden 
}

