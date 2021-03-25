#ifndef MY_MODE_H
#define MY_MODE_H

#include "Handler.h"

#define MODE_STARTUP 0
#define MODE_INITIALIZE_IMU 1
#define MODE_ARM_MOTORS 2
#define MODE_STANDBY 3
#define MODE_MANUAL_CONTROL 4
#define MODE_STABILIZED_CONTROL 5

#define STATUS_STARTUP 0
#define STATUS_POWERING_UP 1
#define STATUS_STANDBY = 2
#define STATUS_READY_FOR_FLIGHT 3


const char* modes[] = {
    "Startup",
    "Initialization of IMU",
    "Arming",
    "Standing by"
    "Manual control",
    "Stabilized control"
};

const char* states[] = {
    "Startup",
    "Powering up",
    "Standby",
    "Ready for flight"
};

uint8_t MODE = MODE_STARTUP;
uint8_t STATUS = STATUS_STARTUP;

void commandMode (byte amountOfParams) {
    if (amountOfParams == 2) {
        MODE = (uint8_t) readInt();
    }
}

#endif