import time
import serial
ser = serial.Serial('/dev/ttyACM0', 500000) # Establish the connection on a specific port

# while True:
#     ser.write(input("Your input: ").encode())
#     print("<Reading now>")
#     print(ser.readline().decode("ascii"))

ser.flush()
# ser.write("Moi\n".encode("ascii"))
# ser.write("Moi\n".encode("ascii"))
# ser.write("Moi\n".encode("ascii"))
# ser.write("Moi\n".encode("ascii"))
# ser.write("Moi\n".encode("ascii"))
# print(ser.readline().strip().decode("ascii"))
# print("listening")
a = 0
while True:
    # enc = (int(input())).to_bytes(2, "little")
    # enc = (3456).to_bytes(2, "little")
    # enc = bytes([13, 4,   4, 0, 5, 0, ])
    enc = bytes([12, 4,   4, 0, 5, 0,   13, 4,   4, 0, 5, 0, ])
    # enc = "AB".encode("ascii")
    # print(list(enc))
    ser.write(enc)
    ser.flush()
    a += 1
    # print("gettling line")
    print(a)
    time.sleep(0.05)
    if ser.inWaiting():
        print(ser.read(ser.inWaiting()).strip().decode("ascii"))
