#ifndef MY_BUTTON_H
#define MY_BUTTON_H

#include <Arduino.h>

class Button {
  private:
    byte pin;

    byte state;
    byte lastReading;

    unsigned long lastDebounceTime = 0;
    unsigned long debounceDelay = 50;

    void initialize ();
  public:
    Button(byte pin);
    void update ();
    bool isPressed ();
};

#endif
