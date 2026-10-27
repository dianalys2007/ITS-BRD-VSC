#include "LCD_GUI.h"
#include "init.h"
#include "input.h"
#include "bmp.h"
#include "perfTimer.h"
#include "stm32f4xx_hal.h"

int main(void)
{
    initITSboard();

    GUI_init(DEFAULT_BRIGHTNESS);

    initInput();
    initPerfTimers();

    static bool newFile = true;

    while (1)
    {
        
        if (!buttonPressed()) {
            continue;
        }
        //openNextFile(); //öffnet die nächste datei

        openNextFile();
        
        GUI_clear(BLACK);

        if (readHeaders() != 0)
        {
            continue;
        }

        if (checkHeaders() != 0)
        {
            continue;
        }

        if (infoHeader.biBitCount == 24)
		//verarbeitet 24-Bit-BMPs, die unkomprimiert sind (BI_RGB)
        {
            drawBMP24();
        }

        else if (infoHeader.biBitCount == 8)
		//verarbeitet 8-Bit-BMPs, die entweder unkomprimiert (BI_RGB) oder mit RLE-Kompression (BI_RLE8) vorliegen können
        {
            if (readColorTable() != 0)
            {
                continue;
            }

            if (infoHeader.biCompression == BI_RGB)
			//unkokomprimiertes 8 bit bmp
            {
                drawBMP8();
            }

            else if (infoHeader.biCompression == BI_RLE8)
			//8 bit bmp mit RLE-Kompression
            {
                drawBMP8RLE();
            }
        }

        HAL_Delay(1000);
    }
}

