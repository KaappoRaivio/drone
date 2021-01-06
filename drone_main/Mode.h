#ifndef MY_MODE_H
#define MY_MODE_H

#include "Handler.h"

#define MODE_STARTUP 0
#define MODE_INITIALIZE_IMU 1
#define MODE_ARM_MOTORS 2
#define MODE_MANUAL_CONTROL 3
#define MODE_STABILIZED_CONTROL 4

#define STATUS_STARTUP 0
#define STATUS_READY_FOR_FLIGHT 1


const char* modes[] = {
    "Startup",
    "Initialize IMU",
    "Arm motors",
    "Manual control",
    "Stabilized control"
};

const char* states[] = {
    "Startup",
    "Ready for flight"
};

uint8_t MODE = MODE_STARTUP;
uint8_t STATUS = STATUS_STARTUP;

void printMode (byte amountOfParams) {
    if (amountOfParams == 2) {
        MODE = (uint8_t) readInt();
    }
    

    Serial.print(F("Currently in mode: "));
    Serial.print(modes[MODE]);
    Serial.print(F(", status "));
    Serial.print(states[STATUS]);
    Serial.print(F(". "));
    Serial.println(millis());
}

#endif