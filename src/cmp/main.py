import time
import serial

import packet
import inputsupplier
# ser = serial.Serial('/dev/ttyACM0', 500000, dsrdtr=)
# print(dir(serial))
ser = serial.Serial()
ser.setDTR(False)
ser.port = "/dev/ttyACM0"
ser.baudrate = 500000
ser.open()
print(ser.dtr)
time.sleep(4)

ser.flush()
a = 0

print("Start")

supplier = inputsupplier.getSupplier()

try: 
    while True:
        a += 1
        enc = bytes()

        if a == 1:
            enc = packet.make_packet(packet.COMMAND_MODE, (1,))
        if a > 1:
            enc = packet.make_packet(packet.COMMAND_IMU, (0,))

        # if a == 40:
        #     enc = packet.make_packet(packet.COMMAND_MODE, (2,))

        # if a == 100:
        #     enc = packet.make_packet(packet.COMMAND_MODE, (4,))
        # if a > 200:
        #     enc += supplier.getInput(False)
        
        ser.write(enc)
        ser.flush()

        line = ser.read(ser.in_waiting).replace(b"\r\n", b"\n").decode("ascii")
        if line:
            print(line, end="")


        time.sleep(0.05)
except KeyboardInterrupt:
    ser.write(packet.make_packet(packet.COMMAND_DISARM_ESC, ()))
