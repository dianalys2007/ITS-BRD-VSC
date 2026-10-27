#ifndef BMP_H
#define BMP_H


#include "BMP_types.h"
#include <stdbool.h>

extern BITMAPFILEHEADER fileHeader;
extern BITMAPINFOHEADER infoHeader;
extern RGBQUAD colorTable[MAX_COLOR_TABLE_SIZE];
extern uint32_t PF7;

int readHeaders(void);
int checkHeaders(void);

int readColorTable(void);

void drawBMP24(void);
void drawBMP8(void);
void drawBMP8RLE(void);
bool buttonPressed(void);

#endif