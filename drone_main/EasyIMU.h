#ifndef MY_EASYIMU_H
#define MY_EASYIMU_H

#include "MPU6050_6Axis_MotionApps20.h"

#define YAW 0
#define PITCH 1
#define ROLL 2


const byte WORKING = 0;
const byte ZEROING = 1;

class EasyIMU {
private:
    byte state;

    MPU6050 mpu;
    uint16_t packetSize;
    uint16_t fifoCount;
    uint8_t fifoBuffer[64];

    Quaternion q;
    VectorFloat gravity;
    VectorInt16 gyro;

    float ypr[3];
    float my_ypr[3];

    // float ypr_offsets[3] = {0.25, -0.08, 1.0};
    float ypr_offsets[3] = {0.0, 0.0, 0.0};

public:
    EasyIMU();
    void init();
    bool isNewDataAvailable();
    float *getNewData();

    void zeroAxes ();
};

EasyIMU::EasyIMU() {}

void EasyIMU::init() {
    TWBR = 24;
    mpu.initialize();
    mpu.dmpInitialize();
    // mpu.CalibrateAccel(10);
    // mpu.CalibrateGyro(10);
    mpu.PrintActiveOffsets();
    mpu.setDMPEnabled(true);

    packetSize = mpu.dmpGetFIFOPacketSize();
    fifoCount = mpu.getFIFOCount();

    // board-specific values
    /*mpu.setXAccelOffset(-3952);
    mpu.setYAccelOffset(1733);
    mpu.setZAccelOffset(1655);
    mpu.setXGyroOffset(-100);
    mpu.setYGyroOffset(-13);
    mpu.setZGyroOffset(58);*/

    
}

bool EasyIMU::isNewDataAvailable() {
    fifoCount = mpu.getFIFOCount();
    if (fifoCount == 1024) {
        mpu.resetFIFO();
        Serial.println(F("FIFO overflow!"));
        return false;
    } else if (fifoCount >= packetSize) {
        if (fifoCount % packetSize != 0) {
            mpu.resetFIFO();
            return false;
        }
        return true;
    } else {
        return false;
    }
}

float *EasyIMU::getNewData() {
    while (fifoCount >= packetSize) {
        mpu.getFIFOBytes(fifoBuffer, packetSize);
        fifoCount -= packetSize;
    }

    mpu.dmpGetQuaternion(&q, fifoBuffer);
    mpu.dmpGetGravity(&gravity, &q);
    mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);

    for (int i = 0; i < 3; i++) {
        my_ypr[i] = ypr_offsets[i] + ypr[i];
    }

    return my_ypr;
}

void EasyIMU::zeroAxes() {
    for (int i = 0; i < 3; i++) {
        ypr_offsets[i] = -ypr[i];
    }
}

#endif