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
        void setParams (float kP, float kI, float kD);
        void resetI ();
};

MyPID::MyPID (float kP, float kI, float kD) {
    time_previous = millis();

    this->kP = kP;
    this->kI = kI;
    this->kD = kD;
}

void MyPID::update (float eP_new) {
    long time = millis();
    float deltaTime = max((time - time_previous) / 1000.0, 0.00001);
    Serial.println(deltaTime, 8);
    time_previous = time;

    // Serial.println(deltaTime);
    
    eP = eP_new;
    eI += eP * deltaTime;
    eD = (eP - eP_previous) / deltaTime;
    // eD = 0;

    // Serial.print(eP);
    // Serial.print(" ");
    // Serial.print(eI);
    // Serial.print(" ");
    // Serial.print(eD);
    // Serial.println(" ");

    eP_previous = eP;
}

float MyPID::getAdjustment () {
    return (eP * kP 
        + eI * kI
        + eD * kD) / 3.0;
}

void MyPID::setParams (float kP, float kI, float kD) {
    this->kP = kP;
    this->kI = kI;
    this->kD = kD;

    resetI();   // usually we want to reset the integral error as its effect is dependent on the gains.
}

void MyPID::resetI () {
    eI = 0;
}

#endif