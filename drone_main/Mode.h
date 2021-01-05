#ifndef MY_MODE_H
#define MY_MODE_H

#include "Handler.h"

const char* modes[] = {
    "Startup",
    "Initialize IMU",
    "Arm motors",
    "Manual control",
    "Stabilized control"
};

uint8_t MODE = 0;

void printMode (byte amountOfParams) {
    if (amountOfParams == 2) {
        MODE = (uint8_t) readInt();
    }

    Serial.print(F("Currently in mode: "));
    Serial.print(modes[MODE]);
    Serial.println(F("."));
}

#endif