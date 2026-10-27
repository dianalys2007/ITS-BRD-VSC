#include "stack.h"
#include "errornew.h"
#include "display.h"
#include "utils.h"

static int stack [STACK_SIZE]; //array für die elemnte
static int top = -1; //zeigt auf das oberste element, also -1 falls leer

int isEmpty(void) //prüft ob stack leer ist
{
    return top == -1;
}

int isFull(void) // prüft ob stack voll ist
{
    return top == STACK_SIZE - 1;
}

int push(int value) //legt einen Wert auf den stack
{
    if (isFull()) {
        return ERROR_STACK_OVERFLOW; //kein platz mehr im stack
    }
    stack[++top] = value; //speichert den wert und erhöht Top
    return ERROR_GOOD; 
}

int pop(int *value) //entfernt das oberste Element vom Stack
{
    if (isEmpty()) {
        return ERROR_STACK_UNDERFLOW; 
    }
    *value = stack[top--]; //gibt den Wert zurück und reduziert Top
    return ERROR_GOOD; 
}

int peekAt(int index, int *value)
{
    if (index < 0 || index > top) {
        return ERROR_STACK_UNDERFLOW;
    }
    *value = stack[index];
    return ERROR_GOOD;
}

int getSize(void)
{
    return top + 1;
}




