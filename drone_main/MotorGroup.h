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

        void setValues (int collective, int pitch, int roll, int yaw);
};

MotorGroup::MotorGroup (uint8_t pin1, uint8_t pin2, uint8_t pin3, uint8_t pin4) : 
    controller1{pin1}, 
    controller2{pin2}, 
    controller3{pin3}, 
    controller4{pin4} {

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

    controller1.setSpeed(speed1);
    controller2.setSpeed(speed2);
    controller3.setSpeed(speed3);
    controller4.setSpeed(speed4);
}

#endif