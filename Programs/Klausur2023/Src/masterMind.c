#include "masterMind.h"
#include "display.h"
#include "random.h"
#include "terminal.h"

void printNumber(char *s, int size, uint8_t *sno, uint8_t *guess)
{
    printStr(s, DEFAULT_COLOR);

    for (int i = 0; i < size; i++) {
        char digit[2];
        digit[0] = guess[i] + '0';
        digit[1] = '\0';

        if (sno[i] == guess[i]) {
            printStr(digit, HIT_COLOR);
        }else {
            printStr(digit, MISS_COLOR);
        }
        
    }
    
}

bool won(int size, uint8_t *sno, uint8_t *guess)
{
    for (int i = 0; i < size; i++) {
        if (sno[i] != guess[i]) {
            return false;
        }
    }
    return true;
}

void genNewNumber(int size, uint8_t *sno)
{
    for (int i = 0; i < size; i++) {
        sno[i] = nxtRandNumber() % 10;
    }
}

// EOF