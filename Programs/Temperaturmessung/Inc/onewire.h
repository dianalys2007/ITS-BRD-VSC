#ifndef ONEWIRE_H
#define ONEWIRE_H

#include "stm32f4xx.h"
#include <stdint.h>
#include <stdbool.h>

extern unsigned char ROM_NO[8];
extern unsigned char crc8;
extern int LastDiscrepancy;

void ow_init(void);

int OWReset(void);
void OWWriteBit(uint8_t bit);
uint8_t OWReadBit(void);
void OWWriteByte(uint8_t val);
uint8_t OWReadByte(void);

int OWFirst(void);
int OWNext(void);
int OWSearch(void);

int read_temperature_by_rom(const uint8_t rom[8], float *out_c);

#endif