#ifndef GRIO_H
#define GRIO_H
#include <stdbool.h>
#include <stdint.h>

void gpio_initEncoderInterrupts(void);

void EXTI0_IRQHandler(void);
void EXTI1_IRQHandler(void);


//eingänge lesen
bool gpio_readA(void);
bool gpio_readB(void);

//leds steuern
void gpio_setForwardLED(void);
void gpio_setBackwardLED(void);
void gpio_setErrorLED(void);

//leds zurücksetzen
void gpio_clearForwardLED(void);
void gpio_clearBackwardLED(void);
void gpio_clearErrorLED(void);

//set leds funktion

//counter ausgeben
void gpio_writeCount(int32_t value);

bool gpio_readS6(void);


void set_pe4(void);
void clear_pe4(void);

#endif