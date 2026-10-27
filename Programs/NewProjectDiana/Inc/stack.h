#ifndef STACK_H
#define STACK_H

#define STACK_SIZE 100

int push(int value);
int pop(int *value);
int peekAt(int index, int *value);
int getSize(void);
int isEmpty(void);
int isFull(void);


#endif

