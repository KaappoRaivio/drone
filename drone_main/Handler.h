#ifndef MY_HANDLER_H
#define MY_HANDLER_H

typedef void (* Handler)(byte);
int readInt () {
    if (Serial.available() >= 2) {
        byte little = Serial.read();
        byte big = Serial.read();

        return (big << 8) + little;
    } else {
        return -1;
    }
}
void handler_add (byte amountOfParams) {
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

void handler_multiply (byte amountOfParams) {
    Serial.print("handler2 received: ");
    Serial.println(amountOfParams);

    int product = 1;
    for (int i = 0; i < amountOfParams / 2; i++)
    {
        product *= readInt();
    }

    Serial.print("Handler2 got: ");
    Serial.println(product);
}



#endif