#ifndef MY_BUFFERSIZE
#define MY_BUFFERSIZE 64
#endif

#ifndef MY_SERIALREADER_H
#define MY_SERIALREADER_H

class MySerialReader {
    private:
        char buffer[MY_BUFFERSIZE];
        int endPointer;

        bool tryReadingLine();
    public:
        MySerialReader ();
        bool isMessageComplete ();
        const char* getMessageBuffer ();
};


#endif