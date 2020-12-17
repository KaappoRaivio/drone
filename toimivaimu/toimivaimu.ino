#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"
#include "Wire.h"

MPU6050 mpu;

uint16_t packetSize;
uint16_t fifoCount;
uint8_t fifoBuffer[64];

Quaternion q;
VectorFloat gravity;
VectorInt16 gyro;

float ypr[3];

void setup() {
  Wire.begin();
  TWBR = 24;
  mpu.initialize();
  mpu.dmpInitialize();

  mpu.setDMPEnabled(true);
  packetSize = mpu.dmpGetFIFOPacketSize();
  fifoCount = mpu.getFIFOCount();

  Serial.begin(115200);

  mpu.setXAccelOffset(-3952);
  mpu.setYAccelOffset(1733);
  mpu.setZAccelOffset(1655);
  mpu.setXGyroOffset(-100);
  mpu.setYGyroOffset(-13);
  mpu.setZGyroOffset(58);

}

int status = 1;
int counter = 0;

void loop() {

  while (fifoCount < packetSize) {

    //insert here your code
    fifoCount = mpu.getFIFOCount();
  }

  if (fifoCount == 1024) {
    mpu.resetFIFO();
    Serial.println(F("FIFO overflow!"));
  }
  else {
    if (fifoCount % packetSize != 0) {
      mpu.resetFIFO();
    }
    else {
      while (fifoCount >= packetSize) {
        mpu.getFIFOBytes(fifoBuffer, packetSize);
        fifoCount -= packetSize;
      }

      mpu.dmpGetQuaternion(&q, fifoBuffer);
      mpu.dmpGetGravity(&gravity, &q);
      mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
      //mpu.dmpGetGyro(&gyro, fifoBuffer);

      Serial.print("ypr\t");
      Serial.print(ypr[0] * 180 / PI);
      Serial.print("\t");
      Serial.print(ypr[1] * 180 / PI);
      Serial.print("\t");
      Serial.print(ypr[2] * 180 / PI);
      Serial.println();

      /*Serial.print("gyro\t");
      Serial.print(gyro.x);
      Serial.print("\t");
      Serial.print(gyro.y);
      Serial.print("\t");
      Serial.print(gyro.z);
      Serial.println();*/

    }

  }

}
