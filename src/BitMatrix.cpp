#include "BitMatrix.h"

BitMatrix::BitMatrix(Led* leds, int argc) {
  this->leds = leds;
  this->argc = argc;
}

void BitMatrix::set (byte number) {
  this->number = number;

  for (int i = 0; i < argc; i++) {
    /*Serial.print(number);
    Serial.print(" ");
    Serial.println(number & 1);*/
    if (number & 1) {
      leds[i].on();
    } else {
      leds[i].off();
    }

    number = number >> 1;
  }
}
