/**
  ******************************************************************************
  * @file    main.c
  * @author  Franz Korf
  * @brief   Kleines Testprogramm fuer neu erstelle Fonts.
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/

#include "stm32f4xx_hal.h"
#include "init.h"
#include "LCD_GUI.h"
#include "LCD_Touch.h"
#include "lcd.h"
#include "fontsFLASH.h"
#include "additionalFonts.h"
#include "error.h"
#include"timer.h"
#include "onewire.h"
#include "ow_delay.h"
#include "crc.h"
#include <stdint.h>
#include <stdbool.h>
#include "init.h"
#include "LCD_GUI.h"
#include "lcd.h"
#include <stdio.h>

int main(void)
{
    initITSboard();
    GUI_init(DEFAULT_BRIGHTNESS);
    initTimer();

    ow_init();

    while (1)
    {

        //GUI_clear(WHITE);
        lcdGotoXY(0, 0);

        int count = 0; //Zähler für die Anzehl der gefundenen Sensoren

        if (OWFirst()) //startet die Suche nach dem ersten Sensor
        {
            do
            {
                count++;

                crc8 = 0;

                //berechnet die crc über die empfangene 64-Bit-ROM-Adresse
                for (int i = 0; i < 8; i++)
                    docrc8(ROM_NO[i]);

                
                char buf[128];
                int p = sprintf(buf, "Sensor %d: ", count);


                for (int i = 0; i < 8; i++)
                    p += sprintf(buf + p, "%02X ", ROM_NO[i]);

                lcdPrintlnS(buf);

                // char dbg[32];
                // sprintf(dbg, "LD:%d", LastDiscrepancy);
                // lcdPrintlnS(dbg);

                float t;

                //liest die Temperatur des aktuell gewählten Sensors 
                if (read_temperature_by_rom(ROM_NO, &t))
                {
                    char tbuf[64];
                    sprintf(tbuf, "T = %.2f C", t);
                    lcdPrintlnS(tbuf);
                }
                else
                {
                    lcdPrintlnS("T read error");
                }

            } while (OWNext()); //sucht nach dem nächsten Sensor, bis keine weiteren Sensoren vorhanden sind 
        }
        else
        {
            lcdPrintlnS("No sensors found");
        }

        delay_ms(2000);
    }

//     while (1)
// {
//     lcdPrintlnS("SEARCH START");

//     if (OWFirst())
//     {
//         lcdPrintlnS("DEVICE FOUND");
//     }
//     else
//     {
//         lcdPrintlnS("NO DEVICES");
//     }

//     delay_ms(2000);
// }

}