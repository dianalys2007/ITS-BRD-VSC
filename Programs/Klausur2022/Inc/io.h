#ifndef _IO_H 
#define _IO_H

#include <stdbool.h>
#include <stdint.h>

bool buttonPressed(int no);

void switchLEDsOn(uint16_t leds);

void switchLEDsOff(uint16_t leds);


#endif