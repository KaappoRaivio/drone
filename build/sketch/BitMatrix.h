#ifndef MY_BITMATRIX_H
#define MY_BITMATRIX_H

#include "Led.h"
#include <Arduino.h>


class BitMatrix {
  private:
    byte number;
    Led* leds;
    int argc;
    
  public:
    BitMatrix (Led* leds, int argc);
    void set (byte number);
};

#endif
