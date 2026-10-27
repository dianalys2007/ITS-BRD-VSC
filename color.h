#ifndef COLOR_H
#define COLOR_H

#include <stdint.h>

uint16_t convert(uint8_t red, uint8_t green, uint8_t blue);

#endif

/*
Warum benutzen wir uint8_t?

Weil:

* BMP exakt Bytegrößen definiert.

In Embedded:

* immer feste Datentypen verwenden
*/