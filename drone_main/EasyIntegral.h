#ifndef MY_EASYINTEGRAL_H
#define MY_EASYINTEGRAL_H

class EasyIntegral {
    private:
        float integrated;
        unsigned long previousTime;

        void init();
    public:
        EasyIntegral ();
        void add (float fx);
        float get ();
        void reset ();
};

EasyIntegral::EasyIntegral () {}
void EasyIntegral::init() {
    previousTime = millis();
}

void EasyIntegral::add (float fx) {
    unsigned long currentTime = millis();
    integrated += fx / 1000. * (currentTime - previousTime);
    //Serial.println(currentTime - previousTime);
    previousTime = currentTime;
}

float EasyIntegral::get () {
    return integrated;
}

void EasyIntegral::reset () {
    integrated = 0;
}

#endif