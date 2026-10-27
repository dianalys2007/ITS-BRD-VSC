#ifndef ANGLE_H
#define ANGLE_H
#include <stdint.h>
#include "timernew.h"

double angle_calculate(int32_t pulseCount);
double velocity_calculateDelta(int32_t deltaPulses, double deltaTime);

#endif

