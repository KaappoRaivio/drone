import time
import serial

import packet
import inputsupplier
# ser = serial.Serial('/dev/ttyACM0', 500000, dsrdtr=)
ser = serial.Serial()
ser.port = "/dev/ttyACM1"
ser.baudrate = 500000
ser.setDTR(False)
ser.open()
print(ser.dtr)
time.sleep(3)

ser.flush()
a = 0

print("Start")

supplier = inputsupplier.getSupplier()

try: 
    while True:
        a += 1
        enc = bytes()
        # if a == 10:
        #     # enc = packet.make_packet(packet.COMMAND_IMU, (1,))
        #     print("Arming")
        #     enc = packet.make_packet(packet.COMMAND_ARM_ESC, ())


        # if a == 60:
        #     enc = packet.make_packet(packet.COMMAND_IMU, (1, ))
        if a > 10:
            enc = packet.make_packet(packet.COMMAND_IMU, (0,))
            enc += packet.make_packet(packet.COMMAND_PRINT_MODE, ())

        if a == 20:
            enc += packet.make_packet(packet.COMMAND_PRINT_MODE, (1,))
        
        # if a > 80:
        #     enc += packet.make_packet(packet.COMMAND_MMANUAL_CONTROL,  supplier.getInput())
        
        ser.write(enc)
        ser.flush()

        # enc = packet.make_packet(packet.COMMAND_IMU, (0,))
        # ser.write(enc)
        # ser.flush()
        
        line = ser.read(ser.in_waiting).strip().decode("ascii")
        
        if line:
            print(line)
        # print(buffers)
        # if a > 100:
        #     values = line.split(" ")
            
        #     for buffer in buffers:
        #         buffer.pop()
            
        #     for i in range(3):
        #         buffers[i].insert(0, float(values[i]))
                
            
        #     print(*[f"{average(l):.2f}" for l in buffers], sep=", ")
        time.sleep(0.05)
except KeyboardInterrupt:
    ser.write(packet.make_packet(packet.COMMAND_DISARM_ESC, ()))
