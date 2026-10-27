#include "bmp.h"
#include "BMP_types.h"
#include "LCD_GUI.h"
#include "LCD_general.h"
#include "color.h"
#include "input.h"
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include "stm32f4xx_hal.h"
#include "lcd.h"
#include "errorhandler.h"


BITMAPFILEHEADER fileHeader;
BITMAPINFOHEADER infoHeader;
RGBQUAD colorTable[MAX_COLOR_TABLE_SIZE];
static COLOR lineBuffer[480];

int readHeaders(void)
{
    if (COMread((char *)&fileHeader, sizeof(BITMAPFILEHEADER), 1)!= 1) {
        //lcdPrintS("ERROR: File header read failed");

        ERR_HANDLER(true, "ERROR: File header read failed");
        return -1;
    }
    if (COMread((char *)&infoHeader, sizeof(BITMAPINFOHEADER), 1)!= 1) {
        //lcdPrintS("ERROR: Info header read failed");
        ERR_HANDLER(true, "ERROR: Info header read failed");
        return -1;
    }
    
    // Debug: print headers
    char debugBuf[50];
    sprintf(debugBuf, "Type: 0x%04X Size: %u", fileHeader.bfType, fileHeader.bfSize);
    lcdPrintS(debugBuf);
    HAL_Delay(1000);
    
    sprintf(debugBuf, "W:%d H:%d Bits:%d", infoHeader.biWidth, infoHeader.biHeight, infoHeader.biBitCount);
    lcdPrintS(debugBuf);
    HAL_Delay(1000);
    
    return 0;
}

int checkHeaders(void)
{
    // BMP Signatur prüfen
    if (fileHeader.bfType != 0x4D42) {
        //lcdPrintS("ERROR: Invalid BMP signature");
        ERR_HANDLER(true, "ERROR: Invalid BMP signature");
        HAL_Delay(1000);
        return -1;
    }

    if (infoHeader.biBitCount != 24) {
        char dbg[40];
        sprintf(dbg, "ERROR: Not 24-bit (%d)", infoHeader.biBitCount);
        //lcdPrintS(dbg);
        ERR_HANDLER(true, "ERROR: Not 24-bit");
        HAL_Delay(1000);
        return -1;
    }

    if (infoHeader.biCompression != 0)
    {
        //lcdPrintS("ERROR: Compressed BMP");
        ERR_HANDLER(true, "ERROR: Compressed BMP");
        HAL_Delay(1000);
        return -1;
    }

    lcdPrintS("Headers OK!");
    HAL_Delay(1000);
    return 0;
}

void drawBMP24(void)
{
    RGBTRIPLE pixel;
 
    int width = infoHeader.biWidth;
    int height = infoHeader.biHeight;
 
    // Detect BMP orientation
    bool isBottomUp = (height > 0);
    if (height < 0)
    {
        height = -height;
    }
 
    // Calculate row size with padding (BMP rows are 4-byte aligned)
    int rowSize = ((width * 3 + 3) / 4) * 4;
    int padding = rowSize - (width * 3);
 
    // LCD screen limits (assuming 320x240 based on your previous code)
    int lcdWidth = 480; 
    int lcdHeight = 320;
 
    // Read ALL rows to maintain UART protocol sync
    for (int rowIdx = 0; rowIdx < height; rowIdx++)
    {
        // Calculate display Y coordinate based on BMP orientation
        int screenY = isBottomUp ? (height - 1 - rowIdx) : rowIdx;
 
        // Read and process all pixels in this row
        for (int x = 0; x < width; x++)
        {
            if (COMread((char *)&pixel, sizeof(RGBTRIPLE), 1) != 1)
            {
                return; // Error or end of stream
            }
 
            // Only store pixels in the buffer if they fit on screen (and inside lineBuffer)
            if (x < lcdWidth && x < 480) 
            {
                // In 24-bit BMPs, the color is stored directly in the pixel struct
                lineBuffer[x] = convert(pixel.rgbtRed, pixel.rgbtGreen, pixel.rgbtBlue);
            }
        }
 
        // Read and skip padding bytes exactly ONCE
        char dummy;
        for (int i = 0; i < padding; i++)
        {
            if (COMread(&dummy, 1, 1) != 1)
            {
                return;
            }
        }
 
        // Draw the assembled line if it is within visible vertical LCD bounds
        if (screenY >= 0 && screenY < lcdHeight) 
        {
            Coordinate tl = { .x = 0, .y = screenY };
            int drawWidth = (width > lcdWidth) ? lcdWidth : width;
            // Ensure we never draw beyond the lineBuffer's capacity
            if (drawWidth > 480) { drawWidth = 480; }
 
            GUI_WriteLine(tl, drawWidth, lineBuffer);
        }
    }
}  

