#include "I2Cdev.h"
#include "Wire.h"

#include "EasyIMU.h"
#include "CommandDispatcher.h"
#include "Handler.h"
#include "BatteryMonitor.h"
#include "MotorGroup.h"
#include "MyPID.h"
#include "Mode.h"


CommandDispatcher dispatcher;

MotorGroup motorGroup(3, 6, 11, 9);
MyPID PID_pitch(600, 200, 800);
MyPID PID_roll(600, 200, 800);
MyPID PID_yaw(0, 0, 0);

float target_pitch = 0;
float target_roll = 0.0;
float target_yaw = 0.0;
float target_collective = 0;

#define STATE_NOT_ARMED 0
#define STATE_ARMED 1

int state = STATE_NOT_ARMED;

void setup() {
    Wire.begin();
    Serial.begin(500000);
    imu.init();   // TODO: move to a place that makes more sense
}

void armESC (byte amountOfParams) {
    motorGroup.arm();
    state = STATE_ARMED;
}


void manualControl (byte amountOfParams) {
    int scale = readInt();
    int collective = readInt() - scale;
    int pitch = readInt() - scale;
    int roll = readInt() - scale;
    int yaw = readInt() - scale;
    target_collective = collective;
}

void disarmESC (byte amountOfParams) {
    motorGroup.stop();
}

Handler handlers[6] = {
    handleIMUCommand,
    getBatteryVoltages,
    armESC,
    manualControl,
    disarmESC,
    printMode
};

float* eulerAngles;

void loop() {
    if (imu.isNewDataAvailable()) {
        eulerAngles = imu.getNewData();

        if (state == STATE_ARMED) {
            PID_pitch.update(target_pitch - eulerAngles[ROLL]);
            PID_roll.update(target_roll - eulerAngles[PITCH]);

            float adjustment_pitch = PID_pitch.getAdjustment();
            float adjustment_roll = PID_roll.getAdjustment();

            motorGroup.setValues(target_collective, adjustment_pitch, adjustment_roll, 0);
        }
    }

    byte command = dispatcher.getNextCommand();
    if (command != NO_COMMAND) {
        byte amountOfParams = dispatcher.getAmountOfParamBytes();
        handlers[command](amountOfParams);
    }

    motorGroup.tick();
    delay(10);
}
