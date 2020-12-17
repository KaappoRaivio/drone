#include "Arduino.h"


void setup()
{
    Serial.begin(9600);
    //Serial.println("Ready");
    // Serial.setTimeout(100);
}



void loop()
{
    // delay(10);

    // if (Serial.available() > 2)
    // {
    //     byte control = Serial.read();
    //     byte amountOfParams = Serial.read();

    //     while (Serial.available() < amountOfParams)
    //         ;

    //     handlers[control - 12](amountOfParams);
    // }
}