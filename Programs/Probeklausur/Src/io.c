/**
 ******************************************************************************
 * @file    io.c
 *
 * @date    25. Juli 2024
 * @brief   Template der io.c für die Laborklausur GS SoSe 2024
 *
 * @author
 *
 ******************************************************************************
 */

#include "io.h"
#include "stm32f4xx_hal.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

bool buttonPressed(int n)
{
	if (n < 0 || n > 7) {
		return false;
	}

	return (GPIOF -> IDR & (1U << n)) == 0; //active low

}

void setLeds(bool *leds)
{
	if (leds == NULL) {
		return;
	}
	for(int i = 0; i < 8; i++)
	{
		if(leds[i] == true)
		{
			GPIOD -> BSRR = (1U << i);

		}else {
			GPIOD -> BSRR = (1U << (i + 16));
		}
	}
}


void ledOn(int n)
{
	if (n < 0 || n > 7) {
		return;
	}
	GPIOD -> BSRR = (1U << n);

}

void ledOff(int n)
{
	if (n < 0 || n > 7) {
		return;
	}

	GPIOD -> BSRR = (1U << (n +16));

}

void ledWrite(int n, bool on)
{
	if (n < 0 || n > 7) {
		return;
	}

	if (on) {
		GPIOD->BSRR = (1U << n);
	}else {
		GPIOD->BSRR = (1U << (n +16));
	}
	
}

void setAllLeds(bool on)
{
	if (on) {
		for (int i = 0; i < 8; i++) {
			GPIOD->BSRR = (1U << i);
		}
	}else {
		for (int i = 0; i < 8; i++) {
			GPIOD->BSRR = (1U << (i +16));
		}
		
	}
}

void setLEDPattern(uint8_t pattern)
{
	for (int i = 0; i < 8; i++) {
		if (pattern & (1U << i)) {
			GPIOD->BSRR = (1U << i);
		}else{
			GPIOD->BSRR = (1U << (i +16));
		}
	}
}

void toggleLED(int n)
{
	if (n < 0 || n > 7) {
		return;
	}

	if (GPIOD->ODR & (1U << n)) {
		GPIOD->ODR &= ~(1U << n);
	}else {
		GPIOD->ODR |= (1U << n);
	}
}

void configureLEDsAsOutput(void)
{
	
	for(int i = 0; i < 8; i++)
	{
		uint32_t mask = 3U << (2 * i);
		GPIOD->MODER &= ~mask;
		GPIOD->MODER |= 1U << (2 * i);
	}
}

bool buttonIsHigh(int n)
{
	if((GPIOF->IDR & (1U << n))!= 0)
	{
		return true;
	}
	else {
		return false;
	}
}

bool buttonPressedActiveHigh(int n)
{
	if (n < 0 || n > 7) {
		return false;
	}


	return (GPIOF->IDR & (1U << n))!= 0;
}

bool buttonPressedActiveLow(int n)
{
	if (n < 0 || n > 7) {
		return false;
	}

	return (GPIOF->IDR & (1U << n)) == 0;
}

int getFirstPressedButton(void)
{
	for (int i = 0; i < 8; i++) {
		if ((GPIOF->IDR & (1U << i))!= 0) {
			return i;
		}	
	}
	return -1;
}

uint8_t getButtonMask(void)
{
	uint8_t mask = 0;
	for (int i = 0; i < 8; i++) {
		if ((GPIOF->IDR & (1U << i))!= 0) {
			mask |= (1U << i);
		}
	}

	return mask;
}

void copyButtonsToLEDs(void){

	for (int i = 0; i < 8; i++) {
		if ((GPIOF->IDR & (1U << i))!= 0) {
			GPIOD->BSRR = (1U << i);
		}else {
			GPIOD->BSRR = (1U <<(i + 16));
		}
	}
	
}

void ledOnWhileButtonPressed(int button, int led)
{
	if(button < 0 || button > 7 || led < 0 || led > 7 )
	{
		return;
	}

	if ((GPIOF->IDR & (1U << button)) != 0) {
		GPIOD->BSRR = (1U << led);
	}else {
		GPIOD->BSRR = (1U <<(led + 16));
	}
}

void initOneLedandButton(void)
{
	uint32_t mask = 3U << (2 * 0);
	GPIOF->MODER &= ~mask;
	
	GPIOD->MODER &= ~mask;
	GPIOD->MODER |= (1U << (2 * 0));
}

void initLEDsAndButtons(void)
{
	for (int i = 0; i < 8; i++) {
		uint32_t mask = 3U << (2 * i);
		GPIOF->MODER &= ~mask;
	
		GPIOD->MODER &= ~mask;
		GPIOD->MODER |= (1U << (2 * i));
	}
}

void binaryCounterOnLEDs(uint8_t value)
{
	for (int i = 0; i < 8; i++) {
		if (value & (1U << i)) {
			GPIOD->BSRR = (1U << i);
		}else {
			GPIOD->BSRR = (1U << (i + 16));
		}
	}
}

void shiftLEDsWithButtons(void)
{
	static uint8_t pattern = 1;

	if ((GPIOF->IDR & (1U << 0)) != 0) {
		pattern <<= 1;
	}

	if ((GPIOF->IDR & (1U << 1)) != 0) {
		pattern >>= 1;
	}

	if (pattern == 0) {
		pattern = 0;
	}

	for (int i = 0; i < 8; i++) {
		if (pattern & (1U << i)) {
			GPIOD->BSRR = (1U << i);
		}else {
			GPIOD->BSRR = (1U << (i + 16));
		}
	}
}
