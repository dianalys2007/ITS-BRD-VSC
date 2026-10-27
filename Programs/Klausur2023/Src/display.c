
#include "display.h"
#include "LCD_GUI.h"
#include "LCD_Touch.h"
#include "keypad.h"
#include "terminal.h"
#include <stdbool.h>

static Terminal terminal;

void initDisplay(void)
{
    GUI_init(DEFAULT_BRIGHTNESS);
    TP_Init(false);
    terminal = makeKeyPad();
}   

void printStr(char * s, COLOR c)
{
    setTextColorTerm(c, &terminal);
    printTerm(s, &terminal);
}

void clearDisplay(void)
{
    clearTerm(&terminal);
}

char nextInput(bool resetOnly, int size, uint8_t *a)
{
    if (resetOnly) {
        while (1) {
            char key = getKeyPadInput();

            if(key == RESET_KEY || key == RESET_AND_HELP_KEY)
            {
                return key;
            }
        }
    }

    int count = 0;

    while (1) {
        char key2 = getKeyPadInput();

        if(key2 == RESET_KEY || key2 == RESET_AND_HELP_KEY)
        {
            return key2;
        }else if (key2 >= '0' && key2 <= '9') {
            int number = key2 - '0';

            if (count < size) {
                a[count] = number;
                count++;
                printChar(key2, &terminal);
            }
        }else if (count == size && key2 == NEXT_GUESS_READ ) 
        {
            return NEXT_GUESS_READ;
        }
    }
    
}

// EOF