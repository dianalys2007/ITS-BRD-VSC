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

#define PF7 (1U << 7)

BITMAPFILEHEADER fileHeader; //Dateiinformationen
BITMAPINFOHEADER infoHeader; // Bildinformationen

RGBQUAD colorTable[MAX_COLOR_TABLE_SIZE]; //Farbpalette für 8 Bit BMPs

static COLOR lineBuffer[480]; //Zwischenspeicher für eine zeile des Bildes

int readHeaders(void) //liest den Dateiheader und den Infoheader aus der bmp Datei
{

 /*
 Mit &fileHeader übergebe ich die Speicheradresse der Struktur, damit die gelesenen Daten direkt dort gespeichert werden.
sizeof(BITMAPFILEHEADER) gibt die Größe des Headers in Byte an.
Der Rückgabewert wird geprüft. Wenn nicht genau ein Header gelesen wurde, liegt ein Lesefehler vor.
*/

    if (COMread((char *)&fileHeader, sizeof(BITMAPFILEHEADER), 1) != 1) 
    //muss 1 sein, sonst konnte der header nicht vollständig gelesen werden
    {
        //lcdPrintS("ERROR: FileHeader");
        ERR_HANDLER(true, "ERROR: FileHeader");
        return -1;
    }
    {
        //lcdPrintS("ERROR: FileHeader");
        ERR_HANDLER(true, "ERROR: FileHeader");
        return -1;
    }

    if (COMread((char *)&infoHeader, sizeof(BITMAPINFOHEADER), 1) != 1)
    //enthält wichtige Informationen über das Bild, wie z.B. die Breite, Höhe, Farbtiefe und Kompression
    {
        //lcdPrintS("ERROR: InfoHeader");
        ERR_HANDLER(true, "ERROR: InfoHeader");
        return -1;
    }

    // char debugBuf[60];

    // sprintf(debugBuf, "W:%ld H:%ld", infoHeader.biWidth, infoHeader.biHeight);

    // lcdPrintS(debugBuf);

    // HAL_Delay(1000);

    // sprintf(debugBuf, "Bits:%d Comp:%ld", infoHeader.biBitCount, infoHeader.biCompression);

    // lcdPrintS(debugBuf);

    // HAL_Delay(1000);

    return 0;
}

int checkHeaders(void)
{
    if (fileHeader.bfType != 0x4D42)
    /**
        * Prüfung, ob es sich um eine gültige BMP-Datei handelt
        * 0x4D42 entspricht den ASCII-Zeichen 'BM', die am Anfang einer BMP-Datei stehen sollten
     */
    {
        //lcdPrintS("ERROR: BMP");
        ERR_HANDLER(true, "ERROR: BMP");    
        HAL_Delay(1000);
        return -1;
    }

    if (infoHeader.biBitCount != 24 && infoHeader.biBitCount != 8)
    /**
        * Nur 24 Bit und 8 Bit werden unterstützt
    */
    {
        //lcdPrintS("ERROR: BitDepth");
        ERR_HANDLER(true, "ERROR: BitDepth");
        HAL_Delay(1000);
        return -1;
    }

    if (infoHeader.biCompression != BI_RGB && infoHeader.biCompression != BI_RLE8)
    /**
        *Unterstützt werden unkomprimierte BMPs (BI_RGB) und 8-Bit BMPs mit RLE-Kompression (BI_RLE8)
    */
    {
        //lcdPrintS("ERROR: Compression");
        ERR_HANDLER(true, "ERROR: Compression");    
        HAL_Delay(1000);
        return -1;
    }

    lcdPrintS("Headers OK!");

    HAL_Delay(1000);

    return 0;
}

int readColorTable(void)
{
    uint32_t numColors;

    if (infoHeader.biClrUsed == 0)
    /*
        *Wenn keine Palettengröße angegeben ist, wird die Standardgröße von 256 farben verwendet
    */
    {
        numColors = 256;
    }
    else
    {
        numColors = infoHeader.biClrUsed;
    }

    if (COMread((char *)colorTable, sizeof(RGBQUAD), numColors) != numColors)
    /**
        * Lesen der Farbpalette
    */
    {
        lcdPrintS("ERROR: Palette");

        HAL_Delay(1000);

        return -1;
    }

    lcdPrintS("Palette OK");

    HAL_Delay(1000);

    return 0;
}


void drawBMP24(void)
{
    RGBTRIPLE pixel;

    int width = infoHeader.biWidth; //Breite des Bildes in Pixeln
    int height = infoHeader.biHeight; //Höhe des Bildes in Pixeln. 

    bool isBottomUp = (height > 0); //prüfen ob das BMP bottom up gespechert ist

    if (height < 0)
    {
        height = -height;
    }

    int rowSize = ((width * 3 + 3) / 4) * 4; //zeilengröße inklusive 4 byte ausrichtung berechnen 

    int padding = rowSize - (width * 3); //anzahl der padding bytes am ende jeder zeile berechnen

    //LCD auflösung 
    int lcdWidth = 480;
    int lcdHeight = 320;

    for (int rowIdx = 0; rowIdx < height; rowIdx++) 
    //alle bildzeilen nacheinander lesen und zeichnen
    {
        int screenY;

        if (isBottomUp)
        //wenn das BMP bottom up gespeichert ist, muss die Zeile von unten nach oben gezeichnet werden
        {
            screenY = height - 1 - rowIdx;
        }
        else
        {
            screenY = rowIdx;
        }

        for (int x = 0; x < width; x++) 
        //alle Pixel der Zeile lesen
        {
            if (COMread((char *)&pixel, sizeof(RGBTRIPLE), 1) != 1)
            //prüfen ob das Pixel erfolgreich gelesen wurde
            {
                lcdPrintS("ERROR: Pixel");

                return;
            }

            if (x < lcdWidth)
            //nur die Pixel zeichnen, die auf den Bildschirm passen
            {
                lineBuffer[x] = convert(pixel.rgbtRed,pixel.rgbtGreen,pixel.rgbtBlue);
                //die Funktion convert wandelt die 24-Bit RGB Werte in das 16-Bit RGB565 Format um, das vom LCD verwendet wird
            }
        }

        //padding bytes am Ende der Zeile überspringen
        char dummy;

        for (int i = 0; i < padding; i++)
        {
            //die padding bytes müssen gelesen werden, um zum Anfang der nächsten Zeile zu gelangen, aber sie enthalten keine Bildinformationen, 
            // daher werden sie in eine Dummy-Variable gelesen
            COMread(&dummy, 1, 1);
        }

        if (screenY >= 0 && screenY < lcdHeight)
        //die Zeile zeichnen, wenn sie auf den Bildschirm passt
        {
            Coordinate p =
            {
                //die Zeile wird am linken Bildschirmrand (x=0) und der berechneten y-Position gezeichnet
                .x = 0,
                .y = screenY
            };

            int drawWidth = width;

            if (drawWidth > lcdWidth)
            //wenn die Zeile breiter als der Bildschirm ist, muss die Breite auf die Bildschirmgröße begrenzt werden
            {
                drawWidth = lcdWidth;
            }

            //komplette zeile auf das display zeichnen
            GUI_WriteLine(p,drawWidth,lineBuffer);
        }
    }

    lcdPrintS("BMP24 DONE");
}

void drawBMP8(void)
{
    int width = infoHeader.biWidth; //Breite des Bildes in Pixeln
    int height = infoHeader.biHeight; //Höhe des Bildes in Pixeln

    int rowSize = ((width + 3) / 4) * 4; //Zeilengröße inklusive 4-Byte-Ausrichtung berechnen. Jede Zeile muss auf ein Vielfaches von 4 Byte ausgerichtet sein.

    int padding = rowSize - width; //Anzahl der Padding-Bytes am Ende jeder Zeile berechnen. Diese Bytes müssen gelesen, aber ignoriert werden, da sie keine Bildinformationen enthalten.

    for (int y = height - 1; y >= 0; y--)
    //bmp wird von unten nach oben gelesen
    {
        for (int x = 0; x < width; x++)
        //alle Pixel der Zeile lesen
        {
            int pixelIndex = nextChar(); //farbindex aus der datei lesen. Dieser index verweist auf die farbe in der farbtabelle

            if (pixelIndex < 0)
            {
                lcdPrintS("ERROR: Pixel");

                return;
            }

            RGBQUAD color = colorTable[pixelIndex]; //Farbe aus der Farbpalette holen

            if (x < 480)
            //nur die Pixel zeichnen, die auf den Bildschirm passen
            {
                lineBuffer[x] = convert( color.rgbRed, color.rgbGreen, color.rgbBlue);
                    
            }
        }

        for (int i = 0; i < padding; i++)
        //Padding-Bytes am Ende der Zeile überspringen
        {
            nextChar();
        }

        if (y < 320)
        //die Zeile zeichnen, wenn sie auf den Bildschirm passt
        {
            Coordinate p =
            {
                .x = 0,
                .y = y
            };

            int drawWidth = width;

            if (drawWidth > 480)
            //wenn die Zeile breiter als der Bildschirm ist, muss die Breite auf die Bildschirmgröße begrenzt werden
            {
                drawWidth = 480;
            }

            //komplette zeile auf das display zeichnen
            GUI_WriteLine(p,drawWidth,lineBuffer);
        }
    }

    lcdPrintS("BMP8 DONE");
}

void drawBMP8RLE(void)
{
    int currentX = 0; //aktuelle x-Position des zu zeichnenden Pixels
    int currentY = infoHeader.biHeight - 1;//start unten links im bild 

    while (1)
    //Endlosschleife, die durch spezielle RLE-Codes unterbrochen wird, wenn das Ende des Bildes erreicht ist
    {
        //zwei bytes lesen
        int byte1 = nextChar();
        int byte2 = nextChar();

        if (byte1 < 0 || byte2 < 0)
        {
            lcdPrintS("RLE ERROR");

            return;
        }

        if (byte1 != 0)
        {
            RGBQUAD color = colorTable[byte2];//farbe aus der farbtabelle holen

            COLOR rgb565 = convert(color.rgbRed,color.rgbGreen,color.rgbBlue);

            for (int i = 0; i < byte1; i++)
            //byte1 gibt an, wie oft die Farbe wiederholt werden soll. Es werden byte1 Pixel mit der Farbe aus byte2 gezeichnet
            {
                if (currentX < 480 && currentY < 320)
                //nur die Pixel zeichnen, die auf den Bildschirm passen
                {
                    Coordinate p =
                    {
                        .x = currentX,
                        .y = currentY
                    };

                    //die Funktion GUI_drawPoint zeichnet einen einzelnen Punkt auf dem Bildschirm. Sie erhält die Koordinaten des Punkts, die Farbe im RGB565-Format, 
                    // die Größe des Punkts (DOT_PIXEL_1X1 für einen einzelnen Pixel) und die Fülloption (DOT_FILL_AROUND, um den Punkt vollständig zu füllen).
                    GUI_drawPoint(p,rgb565,DOT_PIXEL_1X1,DOT_FILL_AROUND);

                    ///WRITELINE ersetzen !!!!!!!!!!!!!
                }

                currentX++; //x-Position für das nächste Pixel erhöhen
            }
        }

        else //wenn byte1 0 ist, handelt es sich um einen speziellen RLE-Code, der nicht die Wiederholung einer Farbe angibt, sondern eine Steueranweisung für die Position oder das Ende des Bildes
        {
            if (byte2 == 0)
            //Zeilenumbruch: Wenn byte2 0 ist, bedeutet dies, dass die aktuelle Zeile beendet ist und die nächste Zeile begonnen werden soll.
            {
                currentX = 0;
                currentY--;
            }

            else if (byte2 == 1)
            //Ende des Bildes: Wenn byte2 1 ist, signalisiert dies, dass das Ende des Bildes erreicht ist und keine weiteren Pixel mehr gezeichnet werden müssen. Die Schleife wird mit break verlassen.
            {
                break;
            }

            else if (byte2 == 2)
            //Pixelverschiebung: Wenn byte2 2 ist, folgt eine Pixelverschiebung, die angibt, dass die aktuelle Position um eine bestimmte Anzahl von Pixeln nach rechts (dx) und nach oben (dy) verschoben werden soll.
            {
                int dx = nextChar();
                int dy = nextChar();

                currentX += dx;
                currentY -= dy;
            }

            else
            //absolute Mode: Wenn byte2 größer als 2 ist, handelt es sich um den absoluten Modus, bei dem die nächsten byte2 Pixel
            //direkt in der Datei angegeben sind und gezeichnet werden müssen. Es werden byte2 Farbindexwerte gelesen, die jeweils 
            // auf eine Farbe in der Farbpalette verweisen, und diese Farben werden nacheinander gezeichnet.
            {
                for (int i = 0; i < byte2; i++)
                {
                    int pixelIndex = nextChar();

                    RGBQUAD color = colorTable[pixelIndex];

                    COLOR rgb565 = convert(color.rgbRed,color.rgbGreen,color.rgbBlue);

                    if (currentX < 480 && currentY < 320)
                    {
                        Coordinate p =
                        {
                            .x = currentX,
                            .y = currentY
                        };

                        GUI_drawPoint(p,rgb565,DOT_PIXEL_1X1,DOT_FILL_AROUND);
                    }

                    currentX++;
                }

                if (byte2 % 2 != 0)
                //Wenn byte2 ungerade ist, muss ein zusätzliches Padding-Byte gelesen und ignoriert werden, um die 4-Byte-Ausrichtung der Zeilen beizubehalten.
                {
                    nextChar();
                }
            }
        }
    }

    lcdPrintS("RLE DONE");
}


bool buttonPressed(void)
{
    static uint8_t lastState = 0;
    uint8_t currentState;
    if(GPIOF->IDR & PF7)
    {
        currentState = 1;
    }
    else
    {
        currentState = 0;
    }
    if(currentState == 0 && lastState == 1)
    {
        lastState = currentState;
        return true;
    }
    lastState = currentState;
    return false;
} 