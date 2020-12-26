#ifndef MY_COMMANDDISPATCHER_H
#define MY_COMMANDDISPATCHER_H

#define NO_COMMAND 255

enum Command {
    ADD = 0,
    MULTIPLY = 1
};

#define Command enum Command

const byte IN_WAITING = 0;
const byte IN_TRANSIT = 1;
const byte READY = 2;

class CommandDispatcher {
    private:
        byte state = IN_WAITING;
        byte command;
        byte nextMessageLength;
    public:
        CommandDispatcher ();
        byte getNextCommand ();
        byte getAmountOfParamBytes ();
};

CommandDispatcher::CommandDispatcher () {}
byte CommandDispatcher::getNextCommand () {
    if (state == READY) {
        state = IN_WAITING;
    }

    if (state == IN_WAITING && Serial.available() >= 2) {
        nextMessageLength = Serial.read();
        command = Serial.read();
        state = IN_TRANSIT;
    } 

    if (state == IN_TRANSIT && Serial.available() >= nextMessageLength) {
        state = READY;
        return command - 12;
    }

    return NO_COMMAND;
}

byte CommandDispatcher::getAmountOfParamBytes () {
    if (state != READY) {
        Serial.println(F("WARNING: buffer not ready but trying to get param length!!!"));
    }
    return nextMessageLength;
}



#endif