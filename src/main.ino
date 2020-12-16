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