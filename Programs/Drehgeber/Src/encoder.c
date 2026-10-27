#include "encoder.h"
#include "gpio.h"
#include "lcd.h"
#include "stm32f4xx_hal.h"
#include <stdbool.h>
#include <stdint.h>

// typedef enum { //zustände
//     PHASE_A = 0,
//     PHASE_B,
//     PHASE_C,
//     PHASE_D
// } Phase;


// static uint8_t errorCounter = 0;
// static int8_t last_direction = 0;
// static uint32_t step_counter_A = 0;
// static uint32_t step_counter_A_window = 0;


static volatile int32_t pulseCount = 0;//gesamtanzahl der impulse;
static volatile uint32_t lastTimestamp = 0;//zeitstempel der letzten gültigen Zustandswechsel
static volatile Direction direction = DIRECTION_ZERO; //aktuelle drehrichtung
static volatile bool errorFlag = false; //fehlerflag
static volatile uint8_t lastPhase = 0; //letzter erkannter zustand des encoders
static volatile uint8_t current = 0;


static uint8_t getPhase(bool A, bool B)
{
    return ((uint8_t)A << 1) | (uint8_t)B;
}

static const int8_t transition_table[4][4] = {
    /* from\to   00   01   10   11 */
    /* 00 */   {  0, +1, -1, 42 },
    /* 01 */   { -1,  0, 42, +1 },
    /* 10 */   { +1, 42,  0, -1 },
    /* 11 */   { 42, -1, +1,  0 }
};

// static Phase getPhase(bool A, bool B) {//wandelt zwei Signale in einen Zustand 

//     // Umwandlung von A oder B zu Phase
//     if (!A && !B) return PHASE_A;
//     if ( A && !B) return PHASE_B;
//     if ( A &&  B) return PHASE_C;
//     if (!A &&  B) return PHASE_D;

//     return PHASE_A; 
// }

void encoder_init(bool A, bool B) { //initialisierung, startwerte 
    pulseCount = 0;
    lastTimestamp = 0;
    direction = DIRECTION_ZERO;
    errorFlag = false;
    lastPhase = getPhase(A, B);
    current = lastPhase;
}



// static bool validTransition(Phase last, Phase current) {
//     // Überprüfen, ob die Übergänge gültig sind
//     return (
//         (last == PHASE_A && (current == PHASE_B || current == PHASE_D)) ||
//         (last == PHASE_B && (current == PHASE_A || current == PHASE_C)) ||
//         (last == PHASE_C && (current == PHASE_B || current == PHASE_D)) ||
//         (last == PHASE_D && (current == PHASE_A || current == PHASE_C))
//     ) ;
// }



void encoder_update(bool A, bool B, uint32_t timestamp) {

    current = getPhase(A, B);
    int8_t dir = transition_table[lastPhase][current];

    if (dir == 42) 
    {
        errorFlag = true;
        lastPhase = current;
        lastTimestamp = timestamp;
        return;
    }

    if (dir == 0) 
    {
        lastPhase = current;
        lastTimestamp = timestamp;
        return;
    }

    pulseCount += dir;
    lastTimestamp = timestamp;

    if (dir > 0) 
    {
        direction = DIRECTION_FORWARD;
    } else 
    {
        direction = DIRECTION_BACKWARD;
    }

    lastPhase = current;
}

void encoder_getSnapshot(int32_t *count, uint32_t *timestamp)
{
    int32_t c1, c2;
    uint32_t t1, t2;

    do 
    {
        t1 = lastTimestamp;
        c1 = pulseCount;
        t2 = lastTimestamp;
        c2 = pulseCount;
    } while ((t1 != t2) || (c1 != c2));

    *count = c1;
    *timestamp = t1;
}

int32_t encoder_get_pulseCount(void) {
    return pulseCount; // aktuellen Zähler zurückgeben
}

Direction encoder_get_direction(void) {
    return direction; // aktuelle Richtung
}

bool encoder_hasError(void) {
    return errorFlag; // Fehlerstatus
}

uint32_t encoder_get_lastTimestamp(void) {
    return lastTimestamp; // Zeit des letzten Wechsels
}

void encoder_resetError(bool A, bool B) {
    errorFlag = false; // Fehler zurücksetzen
    pulseCount = 0;
    direction = DIRECTION_ZERO;
    lastTimestamp = 0;
    lastPhase = getPhase(A, B);
    current = lastPhase;
}

bool encoder_phaseChanged(bool A, bool B){
    current = getPhase(A,  B);
    return (current != lastPhase);
}




