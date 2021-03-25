import os,sys,inspect
currentdir = os.path.dirname(os.path.abspath(inspect.getfile(inspect.currentframe())))
parentdir = os.path.dirname(currentdir)
sys.path.insert(0,parentdir)

import time
import serial
import packet
import threading


def requires_context_manager(f):
    def _inner(self, *args, **kwargs):
        if not self.in_context_manager:
            raise Exception("Not in context manager!")
        else:
            return f(self, *args, **kwargs)

    return _inner


class DroneCommunication:
    def __init__(self, port="/dev/ttyACM0"):
        self.port = port
        self.in_context_manager = False
        self.reset()

        self.buffer = []

    def __enter__(self):
        self.in_context_manager = True
        self.ser.flush()
        return self

    def __exit__(self, exc_type, exc_val, exc_tb):
        self.ser.write(packet.make_packet(packet.COMMAND_MODE, (packet.MODE_STANDBY,)))
        self.in_context_manager = False

    def reset(self):
        self.ser = serial.Serial()
        self.ser.setDTR(False)
        self.ser.port = self.port
        self.ser.baudrate = 500000
        self.ser.open()
        self.ser.flush()

    @requires_context_manager
    def read_all(self):
        self.buffer.append(self.ser.read(self.ser.in_waiting).replace(b"\r\n", b"\n").decode("ascii"))
        joined = "".join(self.buffer)
        if "\n" in joined:
            lines = joined.split("\n")
            *intact, incomplete = lines

            self.buffer = [incomplete]
            return intact
        else:
            return []

    @requires_context_manager
    def write(self, packet):
        self.ser.write(packet)
        self.ser.flush()


class DroneTelemetryManager(threading.Thread):
    def __init__(self, droneCommunication):
        super().__init__()
        self.droneCommunication = droneCommunication
        self.__run = True

        self.linebuffer = []
        self.telemetry = "No data"

    def stop(self):
        self.__run = False

    def run(self):
        while self.__run:
            self.droneCommunication.write(packet.CommandPackets.PRINT_TELEMETRY)
            lines = self.droneCommunication.read_all() 
            self.linebuffer.extend(lines)

            startindex = -1
            endindex = -1

            latest_packet = []

            index = 0
            while index < len(self.linebuffer):
                line = self.linebuffer[index]

                if line.startswith("; START OF TELEMETRY"):
                    startindex = index

                elif line.startswith("; END OF TELEMETRY"):
                    endindex = index

                    if startindex >= 0 and endindex > 0:
                        latest_packet = self.linebuffer[startindex + 1:endindex]
                        self.linebuffer = self.linebuffer[endindex + 1:]
                        startindex = -1
                        endindex = -1
                        index = 0

                index += 1

            if latest_packet:
                self.telemetry = "\n".join(latest_packet)
                print("TELEMETRY:", self.telemetry)
            
            time.sleep(1)
        
    def hasTelemetry(self):
        return self.telemetry != "No data"


if __name__ == "__main__":
    with DroneCommunication() as c:

        telemetry = DroneTelemetryManager(c)
        telemetry.start()

        counter = 0
        
        enc1 = packet.CommandPackets.SOFT_INITIALIZE
        enc1 += packet.make_packet(packet.COMMAND_IMU, (packet.IMU_RESET,))
        c.write(enc1)
        
        while True:
            

            if counter == 30:
                print("resetting imu")
                c.write(packet.make_packet(packet.COMMAND_IMU, (packet.IMU_RESET,)))


            if telemetry.hasTelemetry():
                counter += 1
            time.sleep(0.1)

