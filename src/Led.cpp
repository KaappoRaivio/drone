#include "Led.h"

Led::Led (byte pin) {
  this-> pin = pin;
  initialize();
}

void Led::initialize () {
  pinMode(pin, OUTPUT);
  off();
}

void Led::on () {
  digitalWrite(pin, HIGH);
}

void Led::off () {
  digitalWrite(pin, LOW);
}
