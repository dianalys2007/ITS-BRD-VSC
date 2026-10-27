#include "io.h"
#include "error.h"
#include "stm32f4xx_hal.h"

void setLEDs(int noLeds)
{
    if (noLeds < 0 || noLeds > 16) {
        Error_Handler();
    }

    for (int i = 0; i < 16; i++) {
        if (i < noLeds) {
            if(i < 8)
            {
                GPIOE->BSRR = (1U << (7 - i));
            }else {
                GPIOD->BSRR = (1U << (15 - i));
            }
        }else {
            if(i < 8)
            {
                GPIOE->BSRR = (1U << (7 - i + 16));
            }else {
                GPIOD->BSRR = (1U << (15 - i + 16));
            }
        }
    }
}