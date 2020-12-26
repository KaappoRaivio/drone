#line 1 "/home/kaappo/git/drone/src/main.ino"
#include "Arduino.h"

#include "Handler.h"





//MySerialReader reader;

void setup() {
    Serial.begin(500000);
    //Serial.println("Ready");
    Serial.setTimeout(100);
}


byte buf[80];

int counter = 0;




void loop() {
    delay(10);

    if (Serial.available() > 2) {
        byte amountOfParams = Serial.read();
        byte control = Serial.read();

        while (Serial.available() < amountOfParams);
        
        handlers[control - 12](amountOfParams);
    }
}
