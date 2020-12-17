// #ifndef MY_EASYIMU_CPP
// #define MY_EASYIMU_CPP

// #include "EasyIMU.h"


// EasyIMU::EasyIMU () {
    
// }

// void EasyIMU::init() {
//     TWBR = 24;
//     //Wire.setTimeout(24);
//     mpu.initialize();
//     mpu.dmpInitialize();
//     mpu.CalibrateAccel(6);
//     mpu.CalibrateGyro(6);
//     mpu.PrintActiveOffsets();
//     mpu.setDMPEnabled(true);
    
//     packetSize = mpu.dmpGetFIFOPacketSize();
//     fifoCount = mpu.getFIFOCount();

//     mpu.setXAccelOffset(-3952);
//     mpu.setYAccelOffset(1733);
//     mpu.setZAccelOffset(1655);
//     mpu.setXGyroOffset(-100);
//     mpu.setYGyroOffset(-13);
//     mpu.setZGyroOffset(58);
// }

// bool EasyIMU::isNewDataAvailable() {
//     fifoCount = mpu.getFIFOCount();
//     if (fifoCount >= packetSize) {
//         if (fifoCount == 1024) {
//             mpu.resetFIFO();
//             Serial.println(F("FIFO overflow!"));
//         } else {
//             if (fifoCount % packetSize != 0) {
//                 mpu.resetFIFO();
//             } else {
//                 return true;
//             }
//         }
//     } else {
//         return false;
//     }
// }

// float* EasyIMU::getNewData () {
//     while (fifoCount >= packetSize) {
//         mpu.getFIFOBytes(fifoBuffer, packetSize);
//         fifoCount -= packetSize;
//     }

//     mpu.dmpGetQuaternion(&q, fifoBuffer);
//     mpu.dmpGetGravity(&gravity, &q);
//     mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);

//     return ypr;
// }

// #endif
