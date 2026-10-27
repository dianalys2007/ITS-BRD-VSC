#include "color.h"
#include <stdint.h>

uint16_t convert(uint8_t red, uint8_t green, uint8_t blue)
{
    return ((red & 0xF8) << 8) | ((green & 0xFC) << 3) | (blue >> 3);
}

/*
0xF8 obere 5 Bit
0xFC 6 Bit 
blue >> 3 obere 5 Bit
*/