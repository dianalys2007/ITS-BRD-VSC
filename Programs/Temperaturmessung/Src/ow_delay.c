#include "ow_delay.h"
#include "stm32f4xx.h"

//Verzögerung in Mikrosekunden
void delay_us(uint32_t us){
    uint32_t start;
    uint32_t ticks;

    start = TIM2->CNT; // Startwert des Timers lesen 
    ticks = us * 90; //Anzahl der Ticks berechnen (180MHz)

    while((TIM2->CNT - start) < ticks)
    {

    } // Warten bis die gewünschte Anzahl an Ticks erreicht ist
}

//Verzögerung in Millisekunden
void delay_ms(uint32_t ms){
    while(ms--){
        delay_us(1000); // 1 ms = 1000 us
    }
}

