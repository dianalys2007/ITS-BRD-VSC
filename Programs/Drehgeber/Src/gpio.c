#include "../Inc/gpio.h"
#include "stm32f429xx.h"
#include "stm32f4xx.h"
#include "timer.h"
#include <stdint.h>
#include "timernew.h"
#include "encoder.h"

#define PIN_A (1U << 0) // PF0 = AUX0
#define PIN_B (1U << 1) // PF1 = AUX1

#define LED_FORWARD (1U << 7) // PE7
#define LED_BACKWARD (1U << 6) // PE6
#define LED_ERROR (1U << 5) // PE5
#define LED_PE4 (1U << 4) // PE4
#define PIN_S6 (1U << 6) // PF6

void set_pe4(void)
{
    GPIOE->BSRR |= LED_PE4; 
}

void clear_pe4(void)
{
    GPIOE->BSRR |= (LED_PE4 << 16); 
}

bool gpio_readA(void){//liest den zustand , true=high, false=low
    return (GPIOF->IDR & PIN_A) != 0; 
}

bool gpio_readB(void){//liest den zustand , true=high, false=low
    return (GPIOF->IDR & PIN_B) != 0; 
}

void gpio_setForwardLED(void){
    GPIOE->BSRR |= LED_FORWARD; // Setzen von PE7
}

void gpio_setBackwardLED(void){
    GPIOE->BSRR |= LED_BACKWARD; // Setzen von PE6
}

void gpio_setErrorLED(void){
    GPIOE->BSRR |= LED_ERROR; // Setzen von PE5
}

void gpio_clearForwardLED(void){
    GPIOE->BSRR |= (LED_FORWARD << 16); // Rücksetzen von PE7
}

void gpio_clearBackwardLED(void){
    GPIOE->BSRR |= (LED_BACKWARD << 16); // Rücksetzen von PE6
}

void gpio_clearErrorLED(void){
    GPIOE->BSRR |= (LED_ERROR << 16); // Rücksetzen von PE5
}

bool gpio_readS6(void){// 0=gedrückt, deshalb prüfe ich das 
    return (GPIOF ->IDR & PIN_S6) == 0;
}

void gpio_initEncoderInterrupts(void)
{
    // Clock für GPIOF und SYSCFG aktivieren
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOFEN;
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

    // PF0 und PF1 als Input
    GPIOF->MODER &= ~((3U << (0 * 2)) | (3U << (1 * 2)));

    // EXTI0 auf PF0 routen
    SYSCFG->EXTICR[0] &= ~(0x0F << (4 * 0));
    SYSCFG->EXTICR[0] |=  (0x05 << (4 * 0));   // Port F

    // EXTI1 auf PF1 routen
    SYSCFG->EXTICR[0] &= ~(0x0F << (4 * 1));
    SYSCFG->EXTICR[0] |=  (0x05 << (4 * 1));   // Port F

    // Interrupt bei steigender und fallender Flanke
    EXTI->RTSR |= PIN_A | PIN_B;
    EXTI->FTSR |= PIN_A | PIN_B;

    // Interrupts unmaskieren
    EXTI->IMR |= PIN_A | PIN_B;

    // Pending Flags löschen
    EXTI->PR = PIN_A | PIN_B;

    // NVIC konfigurieren
    NVIC_SetPriority(EXTI0_IRQn, 5);
    NVIC_SetPriority(EXTI1_IRQn, 5);
    NVIC_EnableIRQ(EXTI0_IRQn);
    NVIC_EnableIRQ(EXTI1_IRQn);

}

void EXTI0_IRQHandler(void)
{
    uint32_t timestamp = timer_getTimestamp();

    EXTI->PR = PIN_A;   // Interrupt bestätigen

    bool A = gpio_readA();
    bool B = gpio_readB();

    encoder_update(A, B, timestamp);
}

void EXTI1_IRQHandler(void)
{
    uint32_t timestamp = timer_getTimestamp();

    EXTI->PR = PIN_B;   // Interrupt bestätigen

    bool A = gpio_readA();
    bool B = gpio_readB();

    encoder_update(A, B, timestamp);
}


