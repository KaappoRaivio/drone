#ifndef MY_MYPID_H
#define MY_MYPID_H

class MyPID {
    private:
        float kP;
        float kI;
        float kD;

        float eP_previous;

        float eP;
        float eI;
        float eD;

        long time_previous;

    public:
        void update (float eP);
        float getAdjustment ();
        MyPID (float kP, float kI, float kD);
};

MyPID::MyPID (float kP, float kI, float kD) {
    time_previous = millis();
}

void MyPID::update (float eP_new) {
    long time = millis();
    long deltaTime = (time - time_previous) / 1000;
    time_previous = time;
    
    eP = eP_new;
    eI += eP * deltaTime;
    eD = (eP - eP_previous) / deltaTime;

    eP_previous = eP;
}

float MyPID::getAdjustment () {
    return (eP * kP 
        + eI * kI
        + eD * kD) / 3.0;
}

#endif