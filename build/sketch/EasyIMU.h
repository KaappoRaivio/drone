#ifndef MY_EASYIMU_H
#define MY_EASYIMU_H

#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"
#include "Wire.h"

extern MPU6050 mpu;

class EasyIMU {
    private:
        

        uint16_t packetSize;
        uint16_t fifoCount;
        uint8_t fifoBuffer[64];

        Quaternion q;
        VectorFloat gravity;
        VectorInt16 gyro;

        float ypr[3];

    public:
        EasyIMU ();
        void init();
        bool isNewDataAvailable ();
        float* getNewData ();
};

#endif
