#ifndef CRC_H
#define CRC_H

#include <stdint.h>

//uint8_t crc8(const uint8_t *data, uint16_t len);
unsigned char docrc8(unsigned char value);
extern unsigned char crc8;

#endif