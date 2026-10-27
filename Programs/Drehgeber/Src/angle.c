#include "../Inc/angle.h"
#include "timernew.h"
#include <stdint.h>
#define PHASES_PER_REV 1200.0 //anzahl der impulse pro umdrehung
#define TIMER_FREQ 90000000.0 // frequenz des Timers in Hz

double angle_calculate(int32_t pulseCount){ // umrechnung in einer winkel
    return ((double)pulseCount / PHASES_PER_REV) * 360.0; // Umrechnung von Pulsanzahl in Grad
}

double velocity_calculateDelta(int32_t deltaPulses,double deltaTime){ //berechnung der winkelgeschw.
    if (deltaTime == 0 || deltaPulses == 0) { //null division
        return 0.0; 
    }
    //double deltaAngle = ((double)deltaPulses/ PHASES_PER_REV) * 360.0; // Umrechnung von Pulsdifferenz in Grad
    double deltaSeconds = deltaTime; // Timer läuft mit 90 MHz

    return ((double)deltaPulses / PHASES_PER_REV) * 360.0 / deltaSeconds; // Geschwindigkeit in Grad pro Sekunde
}

