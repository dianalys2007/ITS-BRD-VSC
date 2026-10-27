#ifndef BMP_H
#define BMP_H

#include <stdint.h>
#include "BMP_types.h"

extern BITMAPFILEHEADER fileHeader;
extern BITMAPINFOHEADER infoHeader;
extern RGBQUAD colorTable[MAX_COLOR_TABLE_SIZE];


int readHeaders(void);
int checkHeaders(void);
void drawBMP24(void);
void readColorTable(void);

void drawBMP8RLE(void);



#endif