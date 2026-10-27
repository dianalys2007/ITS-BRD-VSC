#include"io.h"
#include <stdbool.h>
#include <stdint.h>
#include "error.h"
#include "stm32f4xx_hal.h"

void setLEDs(uint8_t noLeds)
{
    if(noLeds > 16)
    {
        Error_Handler();
    }

    for(int i = 0; i < 16; i++)
    {
        if(i <= noLeds)
        {
            if(i < 8)
            {
                GPIOD->BSRR = (1U << i);
            }else{
                GPIOE->BSRR = (1U << (i - 8));
            }
        }else{
            if(i < 8)
            {
                GPIOD->BSRR = (1U << (i +16));
            }else{
                GPIOE->BSRR = (1U << (i + 8));
            }
        }
    }
}

bool buttonPressed(uint8_t button)
{
    if (button > 8 || button < 0) {
        Error_Handler();
    }
    if ((GPIOF->IDR & (1U << button)) == 0) {
        return true;
    }
    return false;
}

// EOF