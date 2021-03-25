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

float IMUSampleRate = -1.0;

void setup() {
    Wire.begin();
    Serial.begin(500000);
    imu.init();
    Serial.println("; START");
}

void controlAttitude (byte amountOfParams) {
    if (MODE == MODE_STABILIZED_CONTROL && amountOfParams == 10) {
        int scale = readInt();
        int collective = readInt() - scale;
        int pitch = readInt() - scale;
        int roll = readInt() - scale;
        int yaw = readInt() - scale;
        target_collective = collective;
        target_pitch = pitch / 180.0 * PI;
        target_roll = roll / 180.0 * PI;
        target_yaw = yaw;
    } else if (MODE == MODE_MANUAL_CONTROL && amountOfParams == 8) {
        int s1 = readInt();
        int s2 = readInt();
        int s3 = readInt();
        int s4 = readInt();

        if (MODE == MODE_MANUAL_CONTROL) {
            motorGroup.setValuesRaw(s1, s2, s3, s4);
        }
    }
}

void commandESC(byte amountOfParams) {
    if (amountOfParams == 2) {
        bool arm = (bool)readInt();
        if (arm) {
            motorGroup.arm();
            imu.zeroAxes();
        } else {
            motorGroup.stop();
        }
    }
}

void printTelemetry (byte amountOfParams) {
    // MODE
    // STATUS
    // BATTERY
    // ORIENTATION
    // IMU SAMPLE RATE
    // PID_PITCH
    // PIC_ROLL
    Serial.println(F("; START OF TELEMETRY"));

    Serial.print(F("; MODE "));
    Serial.println(modes[MODE]);

    Serial.print(F("; STATUS "));
    Serial.println(states[STATUS]);

    Serial.print(F("; BATTERY "));
    printBatteryVoltages();

    Serial.print(F("; ORIENTATION "));
    imu.printIMU();

    Serial.print(F("; IMU SAMPLE RATE "));
    Serial.println(IMUSampleRate);

    Serial.print(F("; PID_PITCH "));
    PID_pitch.printSelf();

    Serial.print(F("; PID_ROLL "));
    PID_roll.printSelf();

    Serial.println(F("; END OF TELEMETRY\n"));
}

void changePID (byte amountOfParams) {
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

Handler handlers[6] = {
    commandIMU,
    commandESC,
    commandMode,
    controlAttitude,
    printTelemetry,
    changePID,
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
        motorGroup.arm();
        motorsArmed = true;
        MODE = MODE_STARTUP;
    }

    if (MODE == MODE_STANDBY) {
        motorGroup.stop();
        motorsArmed = false;
        MODE = MODE_STANDBY;
    }

    if (imuInitialized && motorsArmed) {
        STATUS = STATUS_READY_FOR_FLIGHT;
    }


    if (imu.isNewDataAvailable()) {
        long step = millis();
        IMUSampleRate = 1000.0 / ((float)(step - t));
        t = step;


        eulerAngles = imu.getNewData();

        if (STATUS == STATUS_READY_FOR_FLIGHT && MODE == MODE_STABILIZED_CONTROL) {
            PID_pitch.update(target_pitch - eulerAngles[ROLL]);   // Imu is mounted such that "IMU roll" is "craft pitch" and vice versa.
            PID_roll.update(target_roll - eulerAngles[PITCH]);    // TODO: make transformation more elegant, make it possibible to have adjustable IMU heading

            float adjustment_pitch = PID_pitch.getAdjustment();
            float adjustment_roll = PID_roll.getAdjustment();

            motorGroup.setValues(target_collective, adjustment_pitch, adjustment_roll, target_yaw);
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
