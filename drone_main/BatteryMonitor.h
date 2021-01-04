#ifndef MY_BATTERYMONITOR_H
#define MY_BATTERYMONITOR_H

#define AMOUNT_OF_CELLS 3

class BatteryMonitor {
    private:
        float coeffs[AMOUNT_OF_CELLS];
        float voltages[AMOUNT_OF_CELLS];
        byte pins[AMOUNT_OF_CELLS];

        void init();

    public:
        float* getVoltages ();
        BatteryMonitor(const byte* pins, const float* coeffs);
};

BatteryMonitor::BatteryMonitor (const byte* pins, const float* coeffs) {
    for (int i = 0; i < AMOUNT_OF_CELLS; i++) {
        this->pins[i] = pins[i];
        this->coeffs[i] = coeffs[i];
    }
    
    init();
}

void BatteryMonitor::init () {
    for (int i = 0; i < AMOUNT_OF_CELLS; i++) {
        pinMode(pins[i], INPUT);
        digitalWrite(pins[i], HIGH);
    }
}

float* BatteryMonitor::getVoltages () {
    float offset = 0;

    for (int i = 0; i < AMOUNT_OF_CELLS; i++) {
        voltages[i] = analogRead(pins[i]) / 1024.0 * 5.0 / coeffs[i] - offset; 
        offset = analogRead(pins[i]) / 1024.0 * 5.0 / coeffs[i];
    }

    return voltages;
}

#endif