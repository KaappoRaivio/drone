#ifndef MY_MOTORGROUP_H
#define MY_MOTORGROUP_H

#include "MotorController.h"

class MotorGroup {
    private:
        MotorController controller1;
        MotorController controller2;
        MotorController controller3;
        MotorController controller4;

    public:
        MotorGroup(uint8_t pin1, uint8_t pin2, uint8_t pin3, uint8_t pin4);
        void arm ();
        void tick ();
        void stop ();

        void setValues (int collective, int pitch, int roll, int yaw);
        void setValuesRaw (int s1, int s2, int s3, int s4);
};

MotorGroup::MotorGroup (uint8_t pin1, uint8_t pin2, uint8_t pin3, uint8_t pin4) : 
    controller1{pin1}, 
    controller2{pin2}, 
    controller3{pin3}, 
    controller4{pin4} {

}

void MotorGroup::stop () {
    controller1.stop();
    controller2.stop();
    controller3.stop();
    controller4.stop();
}

void MotorGroup::arm () {
    controller1.arm();
    controller2.arm();
    controller3.arm();
    controller4.arm();
}

void MotorGroup::tick () {
    controller1.tick();
    controller2.tick();
    controller3.tick();
    controller4.tick();
}

/* ʌ->      <-ʌ
   |4|      |2|
   <-v      v->
      \ xx /
        xx
      / xx \
   <-ʌ      ʌ->
   |3|      |1|
   v->      <-v
*/

void MotorGroup::setValues(int collective, int pitch, int roll, int yaw) {
    int speed1 = collective + roll + pitch + yaw;
    int speed2 = collective + roll - pitch - yaw;
    int speed3 = collective - roll + pitch - yaw;
    int speed4 = collective - roll - pitch + yaw;

    setValuesRaw(speed1, speed2, speed3, speed4);
}

void MotorGroup::setValuesRaw (int s1, int s2, int s3, int s4) {
    controller1.setSpeed(s1);
    controller2.setSpeed(s2);
    controller3.setSpeed(s3);
    controller4.setSpeed(s4);
}

#endif