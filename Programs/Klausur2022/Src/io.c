#include "io.h"
#include "stm32f429xx.h"
#include <stdbool.h>
#include <stdint.h>

bool buttonPressed(int no)
{
    return (GPIOF->IDR & (1U << no)) == 0;
}

void switchLEDsOn(uint16_t leds)
{
    for (int i = 0; i < 16; i++) {
        if (leds & (1U << i)) {
            if (i < 8) {
                GPIOD->BSRR = (1U << i);
            }else {
                GPIOE->BSRR = (1U << (i - 8));
            }
        }
    }
}

void switchLEDsOff(uint16_t leds)
{
    for (int i = 0; i < 16; i++) {
        if (leds & (1U << i)) {
            if (i < 8) {
                GPIOD->BSRR = (1U << (i+16));
            }else {
                GPIOE->BSRR = (1U << (i + 8));
            }
        }
    }
}