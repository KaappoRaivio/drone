# 1 "/home/kaappo/git/drone/src/BitMatrix.cpp"
# 2 "/home/kaappo/git/drone/src/BitMatrix.cpp" 2

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
# 1 "/home/kaappo/git/drone/src/main.ino"
# 2 "/home/kaappo/git/drone/src/main.ino" 2

# 4 "/home/kaappo/git/drone/src/main.ino" 2
# 5 "/home/kaappo/git/drone/src/main.ino" 2

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
