#ifndef ENCODER_H
#define ENCODER_H

#include <stdint.h>
#include <stdbool.h>

typedef enum {
    DIRECTION_UNKNOWN,
    DIRECTION_FORWARD,
    DIRECTION_BACKWARD,
    DIRECTION_ZERO
}Direction;

void encoder_init(bool A, bool B);
void encoder_update(bool A, bool B, uint32_t timestamp);

void encoder_getSnapshot(int32_t *count, uint32_t *timestamp);

int32_t encoder_get_pulseCount(void);
Direction encoder_get_direction(void);
bool encoder_hasError(void);
void encoder_resetError(bool A, bool B);

// uint32_t encoder_get_lastTimestamp(void);
// void encoder_resetError(bool A, bool B);
// bool encoder_phaseChanged(bool A, bool B);
// int32_t encoder_getPulseWindow(void);
// void encoder_resetPulseWindow(void);
// bool encoder_phaseChanged(bool A, bool B);

// uint32_t getWindowSteps(void);

// void resetWindow(void);

#endif 

