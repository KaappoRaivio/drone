#include <Arduino.h>
#line 6 "/home/kaappo/git/drone/src/main.ino"
void handler1(int amountOfParams);
#line 20 "/home/kaappo/git/drone/src/main.ino"
void handler2(int amountOfParams);
#line 39 "/home/kaappo/git/drone/src/main.ino"
void setup();
#line 50 "/home/kaappo/git/drone/src/main.ino"
int readInt();
#line 62 "/home/kaappo/git/drone/src/main.ino"
void loop();
#line 0 "/home/kaappo/git/drone/src/main.ino"
#line 1 "/home/kaappo/git/drone/src/BitMatrix.cpp"
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

#line 1 "/home/kaappo/git/drone/src/main.ino"
#include "Arduino.h"

#include "Handler.h"
#include "MySerialReader.h"

void handler1 (int amountOfParams) {
    Serial.print("handler1 received ");
    Serial.print(amountOfParams);
    Serial.println(" bytes as params");

    int sum = 0;
    for (int i = 0; i < amountOfParams / 2; i++) {
        sum += readInt();
    }

    Serial.print("Handler1 got: ");
    Serial.println(sum);
}

void handler2 (int amountOfParams) {
    Serial.print("handler2 received: ");
    Serial.println(amountOfParams);

    int sum = 1;
    for (int i = 0; i < amountOfParams / 2; i++)
    {
        sum *= readInt();
    }

    Serial.print("Handler2 got: ");
    Serial.println(sum);
}

Handler handlers[2] = {handler1, handler2};


//MySerialReader reader;

void setup() {
    Serial.begin(500000);
    //Serial.println("Ready");
    Serial.setTimeout(100);
}


byte buf[80];

int counter = 0;

int readInt () {
    if (Serial.available() >= 2) {
        byte little = Serial.read();
        byte big = Serial.read();

        return (big << 8) + little;
    } else {
        return -1;
    }
}


void loop() {
    delay(10);

    if (Serial.available() > 2) {
        byte control = Serial.read();
        byte amountOfParams = Serial.read();

        while (Serial.available() < amountOfParams);
        
        handlers[control - 12](amountOfParams);
    }
}
