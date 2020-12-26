#include "EasyIMU.h"
#include "CommandDispatcher.h"
#include "Handler.h"
#include "BatteryMonitor.h"

#include "I2Cdev.h"
#include "Wire.h"

EasyIMU imu;
CommandDispatcher dispatcher;

byte cellPins[AMOUNT_OF_CELLS] = {A0, A1, A2};
float cellCoeffs[AMOUNT_OF_CELLS] = {1.0, 0.36, 0.36};
BatteryMonitor monitor(cellPins, cellCoeffs);



void setup() {
    Wire.begin();
    Serial.begin(500000);
    imu.init();
}

byte counter = 0;
float *eulerAngles;

void printIMU (byte amountOfParams) {
    Serial.print("Euler angles\t");
    Serial.print(eulerAngles[YAW] * 180 / PI);
    Serial.print("\t");
    Serial.print(eulerAngles[PITCH] * 180 / PI);
    Serial.print("\t");
    Serial.print(eulerAngles[ROLL] * 180 / PI);
    Serial.println("\t");
}

void zeroIMU (byte amountOfParams) {
    imu.zeroAxes();
}

void getBatteryVoltages (byte amountOfParams) {
    float* voltages = monitor.getVoltages();

    for (int i = 0; i < AMOUNT_OF_CELLS; i++) {
        // Serial.print("Cell ");
        // Serial.print(i);
        Serial.print(voltages[i]);
        Serial.print(" ");
        // Serial.print(F(" V, "));
    }
    Serial.println();
}

Handler handlers[5] = {
    handler_add, 
    handler_multiply,
    printIMU,
    zeroIMU,
    getBatteryVoltages,
};

void loop() {
    if (imu.isNewDataAvailable()) {
        eulerAngles = imu.getNewData();
    }
    delay(10);

    byte command = dispatcher.getNextCommand();
    // Serial.println(command);
    // byte command = 255;
    if (command != NO_COMMAND) {
        byte amountOfParams = dispatcher.getAmountOfParamBytes();
        handlers[command](amountOfParams);
    } else {
        // Serial.println("No command");
    }
}
