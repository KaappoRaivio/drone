#ifndef MY_MOTORCONTROLLER_H
#define MY_MOTORCONTROLLER_H

#include "ESC.h"

#define ARM_MIN 1000
#define ARM_MAX 1500

#define MIN_LIMIT 1000
#define MAX_LIMIT 1300
#define ALLOWED_STEP 4

class MotorController {
    private: 
        ESC esc;
        uint16_t target;
        uint16_t current;

    public:
        MotorController(uint8_t pin);
        void arm();
        void setSpeed(uint16_t newSpeed);
        void tick ();
        void stop ();
};

MotorController::MotorController (uint8_t pin) : esc{pin, 1000, 2000, 500} {
    
}

void MotorController::arm() {
    esc.arm();
    // for (int oESC = ARM_MIN; oESC <= ARM_MAX; oESC += 10) {
    //     esc.speed(oESC);
    //     delay(10);
    // }

    // for (int oESC = ARM_MAX; oESC >= ARM_MIN; oESC -= 10) {
    //     esc.speed(oESC);
    //     delay(10);
    // }

    // esc.speed(0);
    esc.speed(ARM_MIN);
    // delay(200);
    // esc.speed(ARM_MAX);
    // delay(10);
    // esc.speed(ARM_MIN);
}

void MotorController::setSpeed(uint16_t newSpeed) {
    target = constrain(newSpeed + 1000, MIN_LIMIT, MAX_LIMIT);
}

void MotorController::tick() {
    if (target > current) {
        current += min(ALLOWED_STEP, target - current);
    } else if (target < current) {
        current -= min(ALLOWED_STEP, current - target);
    } else {
        // do nothing
    }

    esc.speed(current);
}

void MotorController::stop () {
    esc.stop();
}
/*
ESC myESC(9, 1000, 2000, 500);  // ESC_Name (ESC PIN, Minimum Value, Maximum Value, Default Speed, Arm Value)
int oESC;

void setup() {
    Serial.begin(9600);
    Serial.println(F("Started arming"));
    myESC.arm();

    for (oESC = ARM_MIN; oESC <= ARM_MAX; oESC += 10) {
        myESC.speed(oESC);
        delay(10);
    }

    for (oESC = ARM_MAX; oESC > ARM_MIN; oESC -= 10) {
        myESC.speed(oESC);
        delay(10);
    }
    delay(7000);

    myESC.speed(1150);
    Serial.println(F("Done"));
}

void loop() {
    while (!Serial.available()) {
    }
    int read = Serial.parseInt();
    delay(100);
    Serial.read();
    myESC.speed(read);
    Serial.print(F("Speed now: "));
    Serial.println(read);

    delay(2000);
}
*/
#endif