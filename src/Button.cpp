#include "Button.h"

#include <Arduino.h>

Button::Button(byte pin) {
  this->pin = pin;
  initialize();
}    

void Button::initialize () {
  pinMode(pin, INPUT);
  lastReading = LOW;
}


void Button::update () {
  byte reading = digitalRead(pin);
  if (reading != lastReading) {
    lastDebounceTime = millis();
  }
  
  if (millis() - lastDebounceTime > debounceDelay) {
    state = reading;
  }
  
  lastReading = reading;
}

bool Button::isPressed () {
  update();
  return state == HIGH;
}
