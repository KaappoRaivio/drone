import time
import serial
ser = serial.Serial('/dev/ttyACM1', 115200) # Establish the connection on a specific port
counter = 32 # Below 32 everything in ASCII is gibberish
while True:
    counter += 1
    ser.write(input().encode())
    ser.flush()
    print(ser.readline().decode("utf-8"))
    
    if counter == 255:
        counter = 32