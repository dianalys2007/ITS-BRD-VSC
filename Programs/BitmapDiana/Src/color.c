#include "color.h"
#include <stdint.h>

uint16_t convert(uint8_t red, uint8_t green, uint8_t blue)
{
    return ((red & 0xF8) << 8) | ((green & 0xFC) << 3) | (blue >> 3);
}

/*
 * RGB888:
 * Rot   = 8 Bit
 * Grün  = 8 Bit
 * Blau  = 8 Bit
 *
 * Insgesamt: 24 Bit pro Pixel
 *
 * RGB565:
 * Rot   = 5 Bit
 * Grün  = 6 Bit
 * Blau  = 5 Bit
 *
 * Insgesamt: 16 Bit pro Pixel
 *
 * Das LCD verwendet RGB565.
 * Deshalb müssen die BMP-Farben vor der Ausgabe umgewandelt werden.

*Nur die oberen 5 Bit von Rot übernehmen
*und an die RGB565-Position verschieben

*Nur die oberen 6 Bit von Grün übernehmen
*und an die RGB565-Position verschieben

*Nur die oberen 5 Bit von Blau übernehmen

*Alle Farbanteile zu einem 16-Bit-Wert zusammensetzen
 */

 

