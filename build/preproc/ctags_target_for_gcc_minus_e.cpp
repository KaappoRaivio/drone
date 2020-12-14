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
void setup()
{
    Serial.begin(115200); // set the baud rate
    Serial.println("Ready"); // print "Ready" once
    Serial.setTimeout(100);
}

String inString = "";

void loop()
{

    if (Serial.available())
    { // only send data back if data has been sent
        inString = Serial.readStringUntil('.'); // read the incoming data
        Serial.print("Received: ");
        Serial.println(inString); // send the data back in a new line so that it is not all one long line
    }

}
