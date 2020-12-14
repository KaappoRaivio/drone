#ifndef MY_LED_H
#define MY_LED_H

#include <Arduino.h>

class Led {
  private:
    byte status;
    byte pin;
    
    void initialize ();
  public:
    Led (byte pin);
    void on();
    void off();
};


#endif
