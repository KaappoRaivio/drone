#include "Arduino.h"

#include "Handler.h"
#include "MySerialReader.h"

void handler1 (const char* bytes) {
    Serial.print("handler1 received: ");
    Serial.println(bytes);
}

void handler2 (const char* bytes) {
    Serial.print("handler2 received: ");
    Serial.println(bytes);
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
    // Serial.print(counter++);
    // Serial.println("moi");
    delay(10);
    // Serial.println(Serial.available());
    // if (Serial.available()) {
        /*byte size = Serial.read();
        Serial.println(size);

        while (Serial.available() < size);

        Serial.readBytes(buf, size);
        buf[size + 1] = 0;
        Serial.write(buf, size);*/
        // Serial.println("moi");
        // String read = Serial.readStringUntil('\n');
        // Serial.println(read);
    // }
    
    // while (Serial.available() > 0) {
    //     byte read = Serial.read();
    //     Serial.write((char) read);
    // }
    if (Serial.available() > 1) {
        byte control = Serial.read();
        if (control == 12) {
            while (Serial.available() < 2);

            int res = readInt();
            Serial.print("Resulting int: ");
            Serial.println(res);
        } else if (control == 13) {
            while (Serial.available() < 2);

            byte one = Serial.read();
            byte two = Serial.read();
            Serial.print("Resulting sum: ");
            Serial.println(one + two);
        } else {
            Serial.println("problem");
        }
    }
    // if (Serial.available() > 2) {
    //     // byte one = Serial.read();
    //     // byte two = Serial.read();
    //     // Serial.println(one + two);
    //     int res = readInt();
    //     Serial.print("Result: ");
    //     Serial.println(res);
    // }
}