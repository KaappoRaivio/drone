import time
import serial

import packet

ser = serial.Serial('/dev/ttyACM0', 500000)
time.sleep(3)

ser.flush()
a = 0

print("Start")

buffers = [[0 for i in range(20)] for a in range(3)]

def average(l):
    return sum(l) / len(l)

while True:
    # print(a, " ", end="", flush=True)
    a += 1

    enc = packet.make_packet(packet.COMMAND_GET_BATTERY_VOLTAGES, ())
    ser.write(enc)
    ser.flush()
    
    line = ser.readline().strip().decode("ascii")
    
    # print(line)
    # print(buffers)
    if a > 100:
        values = line.split(" ")
        
        for buffer in buffers:
            buffer.pop()
        
        for i in range(3):
            buffers[i].insert(0, float(values[i]))
            
        
        print(*[f"{average(l):.2f}" for l in buffers], sep=", ")
    time.sleep(0.05)

