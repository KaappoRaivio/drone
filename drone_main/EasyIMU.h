#ifndef MY_EASYIMU_H
#define MY_EASYIMU_H

#define MPU6050_DMP_FIFO_RATE_DIVISOR 0x02
//#define DEBUG true

#include "MPU6050_6Axis_MotionApps20.h"
#include "EasyIntegral.h"

#include "Handler.h"

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

    Quaternion quaternion;
    VectorFloat gravity;
    VectorInt16 gyro;

    float ypr[3];
    float my_ypr[3];

    float ypr_offsets[3] = {0.0, 0.0, 0.0};
    float zAccelOffset = 0;
    float zAccel = 0;
    
    EasyIntegral verticalSpeedIntegral;
    EasyIntegral heightIntegral;

public:
    EasyIMU();
    void init();
    bool isNewDataAvailable();
    float *getNewData();

    void zeroAxes ();
    void printIMU ();
};

EasyIMU imu;

EasyIMU::EasyIMU() {}

void EasyIMU::init() {
    TWBR = 24;
    mpu.initialize();
    if(mpu.dmpInitialize()) {
        Serial.println(F("Problem initializing!!!! Halting!!!"));
        while (true) {};
    };
    mpu.CalibrateAccel(10);
    mpu.CalibrateGyro(10);
    mpu.PrintActiveOffsets();
    mpu.setDMPEnabled(true);

    packetSize = mpu.dmpGetFIFOPacketSize();
    fifoCount = mpu.getFIFOCount();
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

    mpu.dmpGetQuaternion(&quaternion, fifoBuffer);
    mpu.dmpGetGravity(&gravity, &quaternion);
    mpu.dmpGetYawPitchRoll(ypr, &quaternion, &gravity);

    for (int i = 0; i < 3; i++) {
        my_ypr[i] = ypr_offsets[i] + ypr[i];
    }

    zAccel = ((sin(my_ypr[ROLL]) * mpu.getAccelerationY()
        + sin(my_ypr[PITCH]) * mpu.getAccelerationX()
        + cos(my_ypr[PITCH]) * cos(my_ypr[ROLL]) * mpu.getAccelerationZ()) / 16384.0 - 1.) * 9.81 + zAccelOffset;
    // zAccel = cos(my_ypr[PITCH]) * cos(my_ypr[ROLL]) * mpu.getAccelerationZ() / 16384.0;
    verticalSpeedIntegral.add(zAccel);
    heightIntegral.add(verticalSpeedIntegral.get());

    return my_ypr;
}

void EasyIMU::zeroAxes() {
    for (int i = 0; i < 3; i++) {
        ypr_offsets[i] = -ypr[i];
    }

    zAccelOffset = -zAccel;

    verticalSpeedIntegral.reset();
    heightIntegral.reset();
}

void EasyIMU::printIMU () {
    Serial.print(my_ypr[YAW] * 180 / PI);
    Serial.print(F("\t"));
    Serial.print(my_ypr[PITCH] * 180 / PI);
    Serial.print(F("\t"));
    Serial.println(my_ypr[ROLL] * 180 / PI);
}

#define COMMAND_IMU_RESET 0;

void commandIMU(byte amountOfParams) {
    int command = readInt();
    if (command == 0) {
        imu.zeroAxes();
    }
}

#endif