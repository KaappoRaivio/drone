#include "BatteryMonitor.h"
#include "CommandDispatcher.h"
#include "EasyIMU.h"
#include "Handler.h"
#include "I2Cdev.h"
#include "Mode.h"
#include "MotorGroup.h"
#include "MyPID.h"
#include "Wire.h"

CommandDispatcher dispatcher;

MotorGroup motorGroup(3, 6, 11, 9);
// MyPID PID_pitch(0, 0, 0);
MyPID PID_pitch(200, 800, 40);
MyPID PID_roll(15, 400, 20);
MyPID PID_yaw(0, 0, 0);

float target_pitch = 0;
float target_roll = 0.0;
float target_yaw = 0.0;
float target_collective = 0;


void setup() {
    Wire.begin();
    Serial.begin(500000);
    imu.init();
}

void armESC (byte amountOfParams) {
    motorGroup.arm();
    imu.zeroAxes();
}


void manualControl (byte amountOfParams) {
    int scale = readInt();
    int collective = readInt() - scale;
    int pitch = readInt() - scale;
    int roll = readInt() - scale;
    int yaw = readInt() - scale;
    target_collective = collective;
    target_pitch = pitch / 180.0 * PI;
    target_roll = roll / 180.0 * PI;
    target_yaw = yaw;
}

void debugControl (byte amountOfParams) {
    int s1 = readInt();
    int s2 = readInt();
    int s3 = readInt();
    int s4 = readInt();

    if (MODE == MODE_MANUAL_CONTROL) {
        motorGroup.setValuesRaw(s1, s2, s3, s4);
    }
}

void disarmESC (byte amountOfParams) {
    motorGroup.stop();
}

void handlePIDChange (byte amountOfParams) {
    int axis = readInt();
    int p = readInt();
    int i = readInt();
    int d = readInt();

    if (axis == 0) {
        PID_pitch.setParams(p, i, d);
    } else if (axis == 1) {
        PID_roll.setParams(p, i, d);
    }
}

void resetI (byte amountOfParams) {
    PID_pitch.resetI();
    PID_roll.resetI();
    imu.zeroAxes();
}

Handler handlers[9] = {
    handleIMUCommand,
    getBatteryVoltages,
    armESC,
    manualControl,
    disarmESC,
    printMode,
    debugControl,
    handlePIDChange,
    resetI
};

float* eulerAngles;

bool imuInitialized;
bool motorsArmed;


long t = millis();

void loop() {
    if (MODE == MODE_INITIALIZE_IMU) {
        imuInitialized = true;
        MODE = MODE_STARTUP;
    }

    if (MODE == MODE_ARM_MOTORS) {
        // motorGroup.arm();
        motorsArmed = true;
        MODE = MODE_STARTUP;
    }
    // Serial.println(imuInitialized);
    // Serial.println(motorsArmed);

    if (imuInitialized && motorsArmed) {
        STATUS = STATUS_READY_FOR_FLIGHT;
    }


    if (imu.isNewDataAvailable()) {
        eulerAngles = imu.getNewData();

        if (STATUS == STATUS_READY_FOR_FLIGHT && MODE == MODE_STABILIZED_CONTROL) {
            PID_pitch.update(target_pitch - eulerAngles[ROLL]);
            PID_roll.update(target_roll - eulerAngles[PITCH]);

            float adjustment_pitch = PID_pitch.getAdjustment();
            float adjustment_roll = PID_roll.getAdjustment();

            // motorGroup.setValues(target_collective, adjustment_pitch, adjustment_roll, target_yaw);
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
