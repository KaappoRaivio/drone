void setup()
{
    Serial.begin(115200);      // set the baud rate
    Serial.println("Ready"); // print "Ready" once
    Serial.setTimeout(100);
}

String inString = "";

void loop()
{
    
    if (Serial.available())
    {                                // only send data back if data has been sent
        inString = Serial.readStringUntil('.'); // read the incoming data
        Serial.print("Received: ");
        Serial.println(inString);      // send the data back in a new line so that it is not all one long line
    }
    
}