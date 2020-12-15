#include "MySerialReader.h"
#include "Arduino.h"

MySerialReader::MySerialReader () {
    this->buffer[MY_BUFFERSIZE];
    this->endPointer = 0;
}

bool MySerialReader::tryReadingLine () {
    if (Serial.available()) {
        byte read = Serial.read();
        switch (read) {
            case '\r':
                break;
            case '\n':
                this->endPointer = 0;
                return true;
            default:
                if (this->endPointer < MY_BUFFERSIZE - 2) {
                    this->buffer[this->endPointer++] = read;
                    this->buffer[this->endPointer] = 0;
                }
                break;
        }
    } else {
        return false;
    }
}

bool MySerialReader::isMessageComplete () {
    return this->tryReadingLine();
}

const char* MySerialReader::getMessageBuffer () {
    return this->buffer;
}