#include "EasyIMU.h"
#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"
#include "Wire.h"

// class EasyIMU {
//   private:
//     MPU6050 mpu;
//     uint16_t packetSize;
//     uint16_t fifoCount;
//     uint8_t fifoBuffer[64];

//     Quaternion q;
//     VectorFloat gravity;
//     VectorInt16 gyro;

//     float ypr[3];

//   public:
//     EasyIMU();
//     void init();
//     bool isNewDataAvailable();
//     float *getNewData();
// };

// EasyIMU::EasyIMU() {}

// void EasyIMU::init() {
//   TWBR = 24;
//   mpu.initialize();
//   mpu.dmpInitialize();
//   mpu.CalibrateAccel(6);
//   mpu.CalibrateGyro(6);
//   mpu.PrintActiveOffsets();
//   mpu.setDMPEnabled(true);

//   packetSize = mpu.dmpGetFIFOPacketSize();
//   fifoCount = mpu.getFIFOCount();

//   // board-specific values
//   mpu.setXAccelOffset(-3952);
//   mpu.setYAccelOffset(1733);
//   mpu.setZAccelOffset(1655);
//   mpu.setXGyroOffset(-100);
//   mpu.setYGyroOffset(-13);
//   mpu.setZGyroOffset(58);
// }

// bool EasyIMU::isNewDataAvailable() {
//   fifoCount = mpu.getFIFOCount();
//   if (fifoCount == 1024) {
//     mpu.resetFIFO();
//     Serial.println(F("FIFO overflow!"));
//     return false;
//   } else if (fifoCount >= packetSize) {
//       if (fifoCount % packetSize != 0) {
//         mpu.resetFIFO();
//         return false;
//       }
//       return true;
//   } else {
//     return false;
//   }
// }

// float *EasyIMU::getNewData() {
//   while (fifoCount >= packetSize) {
//     mpu.getFIFOBytes(fifoBuffer, packetSize);
//     fifoCount -= packetSize;
//   }

//   mpu.dmpGetQuaternion(&q, fifoBuffer);
//   mpu.dmpGetGravity(&gravity, &q);
//   mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);

//   return ypr;
// }

EasyIMU imu;

void setup() {
    Wire.begin();
    Serial.begin(115200);
    imu.init();
}

byte counter = 0;
float *eulerAngles;

void loop() {
    if (imu.isNewDataAvailable()) {
        eulerAngles = imu.getNewData();

        Serial.print("Euler angles\t");
        Serial.print(eulerAngles[0] * 180 / PI);
        Serial.print("\t");
        Serial.print(eulerAngles[1] * 180 / PI);
        Serial.print("\t");
        Serial.print(eulerAngles[2] * 180 / PI);
        Serial.println("\t");
    }
}
