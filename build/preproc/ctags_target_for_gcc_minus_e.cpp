# 1 "/home/kaappo/git/drone/toimivaimu/toimivaimu.ino"
# 2 "/home/kaappo/git/drone/toimivaimu/toimivaimu.ino" 2
# 3 "/home/kaappo/git/drone/toimivaimu/toimivaimu.ino" 2

# 5 "/home/kaappo/git/drone/toimivaimu/toimivaimu.ino" 2
/*#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"*/

//#include "EasyIMU.h"

class EasyIMU {
  private:
    MPU6050 mpu;
    uint16_t packetSize;
    uint16_t fifoCount;
    uint8_t fifoBuffer[64];

    Quaternion q;
    VectorFloat gravity;
    VectorInt16 gyro;

    float ypr[3];

  public:
    EasyIMU();
    void init();
    bool isNewDataAvailable();
    float *getNewData();
};

EasyIMU::EasyIMU() {}

void EasyIMU::init() {
  
# 33 "/home/kaappo/git/drone/toimivaimu/toimivaimu.ino" 3
 (*(volatile uint8_t *)(0xB8)) 
# 33 "/home/kaappo/git/drone/toimivaimu/toimivaimu.ino"
      = 24;
  //Wire.setTimeout(24);
  mpu.initialize();
  mpu.dmpInitialize();
  mpu.CalibrateAccel(6);
  mpu.CalibrateGyro(6);
  mpu.PrintActiveOffsets();
  mpu.setDMPEnabled(true);

  packetSize = mpu.dmpGetFIFOPacketSize();
  fifoCount = mpu.getFIFOCount();

  mpu.setXAccelOffset(-3952);
  mpu.setYAccelOffset(1733);
  mpu.setZAccelOffset(1655);
  mpu.setXGyroOffset(-100);
  mpu.setYGyroOffset(-13);
  mpu.setZGyroOffset(58);
}

bool EasyIMU::isNewDataAvailable() {
  fifoCount = mpu.getFIFOCount();
  if (fifoCount == 1024) {
    mpu.resetFIFO();
    Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 57 "/home/kaappo/git/drone/toimivaimu/toimivaimu.ino" 3
                  (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 57 "/home/kaappo/git/drone/toimivaimu/toimivaimu.ino"
                  "FIFO overflow!"
# 57 "/home/kaappo/git/drone/toimivaimu/toimivaimu.ino" 3
                  ); &__c[0];}))
# 57 "/home/kaappo/git/drone/toimivaimu/toimivaimu.ino"
                  )));
    return false;
  } else if (fifoCount >= packetSize) {
      if (fifoCount % packetSize != 0) {
        mpu.resetFIFO();
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

  return ypr;
}

EasyIMU imu;

void setup() {
  Wire.begin();
  Serial.begin(115200);
  imu.init();
}

byte counter = 0;
float *ypr;

void loop() {
  if (imu.isNewDataAvailable())
  {
    ypr = imu.getNewData();

    Serial.print("ypr\t");
    Serial.print(ypr[0] * 180 / 3.1415926535897932384626433832795);
    Serial.print("\t");
    Serial.print(ypr[1] * 180 / 3.1415926535897932384626433832795);
    Serial.print("\t");
    Serial.print(ypr[2] * 180 / 3.1415926535897932384626433832795);
    Serial.println("\t");
  }
}
