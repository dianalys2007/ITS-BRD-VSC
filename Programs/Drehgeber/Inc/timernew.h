#ifndef TIMERNEW_H
#define TIMERNEW_H
#include <stdint.h>

uint32_t timer_getTimestamp(void);
double timer_getDeltaSeconds(uint32_t t1, uint32_t t2);

#endif
