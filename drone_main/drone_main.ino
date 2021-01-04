#include "EasyIMU.h"
#include "CommandDispatcher.h"
#include "Handler.h"
#include "BatteryMonitor.h"
#include "MotorGroup.h"
#include "MyPID.h"

#include "I2Cdev.h"
#include "Wire.h"

EasyIMU imu;
CommandDispatcher dispatcher;

const byte cellPins[AMOUNT_OF_CELLS] = {A0, A1, A2};
const float cellCoeffs[AMOUNT_OF_CELLS] = {1.0, 0.36, 0.36};
BatteryMonitor monitor(cellPins, cellCoeffs);



void setup() {
    Wire.begin();
    Serial.begin(500000);
    imu.init();
}

byte counter = 0;
float *eulerAngles;

void handleIMUCommand (byte amountOfParams) {
    int command = readInt();
    if (command == 0) {
        imu.printIMU();
    } else if (command == 1) {
        imu.zeroAxes();
    }
}

void getBatteryVoltages (byte amountOfParams) {
    float* voltages = monitor.getVoltages();

    for (int i = 0; i < AMOUNT_OF_CELLS; i++) {
        Serial.print(voltages[i]);
        Serial.print(" ");
    }
    Serial.println();
}

MotorGroup motorGroup((uint8_t)3, (uint8_t)6, (uint8_t)11, (uint8_t)9);

void armESC (byte amountOfParams) {
    motorGroup.arm();
}

int scale = 300;

void manualControl (byte amountOfParams) {
    int collective = readInt() - scale;
    int pitch = readInt() - scale;
    int roll = readInt() - scale;
    int yaw = readInt() - scale;
    
    Serial.print(collective);
    Serial.print(" ");
    Serial.print(pitch);
    Serial.print(" ");
    Serial.print(roll);
    Serial.print(" ");
    Serial.println(yaw);

    motorGroup.setValues(collective, pitch, roll, yaw);
}

Handler handlers[4] = {
    handleIMUCommand,
    getBatteryVoltages,
    armESC,
    manualControl
};

MyPID PID_pitch (1, 1, 1);
MyPID PID_roll (1, 1, 1);
MyPID PID_yaw (1, 1, 1);

float target_pitch = 0.0;
float target_roll = 0.0;
float target_yaw = 0.0;

void loop() {
    if (imu.isNewDataAvailable()) {
        eulerAngles = imu.getNewData();
        // imu.printIMU();
    }
    delay(10);

    byte command = dispatcher.getNextCommand();
    // Serial.println(command);
    // byte command = 255;
    if (command != NO_COMMAND) {
        byte amountOfParams = dispatcher.getAmountOfParamBytes();
        handlers[command](amountOfParams);
    } else {
        // Serial.println(F("No command"));
    }

    motorGroup.tick();
}
