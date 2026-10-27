#include "../Inc/encoder.h"
#include <stdint.h>
#include <stdbool.h>
#include "init.h"
#include "LCD_GUI.h"
#include "lcd.h"
#include <stdio.h>
#include "gpio.h"
#include "timer.h"
#include "timernew.h"
#include "angle.h"
#include "encoder.h"
#include <math.h>

static void fastDisplay(double value, int y, bool is_speed)
{
    // set_pe4();
    // static char alt_geschwindikeit[10] = "     000.0";
    // static char alt_winkel[10] = "     000.0";
    // char value_str[10];
    // sprintf(value_str, "%.3f", value);
    
    // for (int i = 0; i < 10 && value_str[i] != '\0'; i++)
    // {
    //     if (is_speed)
    //     {
    //         if (alt_geschwindikeit[i] != value_str[i])
    //         {
    //             alt_geschwindikeit[i] = value_str[i];
    //             lcdGotoXY(18 + i, y);
    //             lcdPrintC(value_str[i]);
    //         }
    //     }
    //     else
    //     {
    //         if (alt_winkel[i] != value_str[i])
    //         {
    //             alt_winkel[i] = value_str[i];
    //             lcdGotoXY(18 + i, y);
    //             lcdPrintC(value_str[i]);
    //         }
    //     }
    // }
    // clear_pe4();

    char value_str[16];
    sprintf(value_str, "%.3f", value);

    lcdGotoXY(18,  y);
    lcdPrintS(value_str);

}

#define MAX_TICKS 0.5
#define MIN_TICKS 0.25
 
int main(void){
 
initITSboard();//initialisiert board
GUI_init(DEFAULT_BRIGHTNESS );//initialisiert display
initTimer();//startet timer

bool A = gpio_readA();
bool B = gpio_readB();

encoder_init(A, B);//initialisiert encoder
gpio_initEncoderInterrupts();

lcdGotoXY(0, 0 );
lcdPrintlnS("Angle:");
lcdGotoXY(0, 1);
lcdPrintlnS("Velocity:");
lcdGotoXY(0,  2);
lcdPrintlnS("Direction:");
 
uint32_t lastTime = 0; //letzte Zeimessung
int32_t lastCount = 0; //letzter zählerwert

encoder_getSnapshot(&lastCount, &lastTime);
 
double currentVelocity = 0.0;//aktuelle geschw.
bool lastButton = false;
 
while (1) {
 
   bool button = gpio_readS6();
   bool buttonPressed = button && !lastButton;

   if (buttonPressed && encoder_hasError()) 
   {
            bool nowA = gpio_readA();
            bool nowB = gpio_readB();
            encoder_resetError(nowA, nowB);
            gpio_clearErrorLED();
            gpio_clearForwardLED();
            gpio_clearBackwardLED();
            lastCount = 0;
            lastTime = timer_getTimestamp();
            currentVelocity = 0.0;
            fastDisplay(0.0, 0, false);
            fastDisplay(0.0, 1, true);
   }

   int32_t count;
   uint32_t timestamp;
   encoder_getSnapshot(&count, &timestamp);
   double angle = angle_calculate(count);
   double deltaTime = timer_getDeltaSeconds(lastTime, timestamp);
   int32_t deltaCount = count - lastCount;

   if (deltaTime >= MIN_TICKS || deltaTime >= MAX_TICKS) 
   {
            currentVelocity = velocity_calculateDelta(deltaCount, deltaTime);
            lastCount = count;
            lastTime = timestamp;
    }
    
    fastDisplay(angle, 0, false);
    fastDisplay(currentVelocity, 1, true);
    
    if (encoder_hasError()) {
        gpio_setErrorLED();
        gpio_clearForwardLED();
        gpio_clearBackwardLED();
    } else
    {
        gpio_clearErrorLED();
        Direction dir = encoder_get_direction();
        if (dir == DIRECTION_FORWARD) 
        {
            gpio_setForwardLED();
            gpio_clearBackwardLED();
        } else if (dir == DIRECTION_BACKWARD)
        {
            gpio_setBackwardLED();
            gpio_clearForwardLED();
        } else
        {
            gpio_clearForwardLED();
            gpio_clearBackwardLED();
        }
    }
    
    lastButton = button;
    GPIOD->ODR = (encoder_get_pulseCount() & 0xFF);
}
}
 
