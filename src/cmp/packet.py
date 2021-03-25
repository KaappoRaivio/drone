COMMAND_IMU = 0
COMMAND_ESC = 1
COMMAND_MODE = 2
COMMAND_CONTROL_ATTITUDE = 3
COMMAND_PRINT_TELEMETRY = 4
COMMAND_CHANGE_PID = 5

IMU_RESET = 0

ESC_DISARM = 0
ESC_ARM = 1

MODE_STARTUP = 0
MODE_INITIALIZE_IMU = 1
MODE_ARM_MOTORS = 2
MODE_STANDBY = 3
MODE_MANUAL_CONTROL = 4
MODE_STABILIZED_CONTROL = 5



POSTAMBLE = ord(".")
PREAMBLE = ord("C")



def make_packet(command, params):
    return bytes([PREAMBLE, len(params) * 2, (command + 12) % 256, *[item for sublist in [list(param.to_bytes(2, "little")) for param in params] for item in sublist], POSTAMBLE])

class CommandPackets:
    SOFT_INITIALIZE = make_packet(COMMAND_MODE, (MODE_INITIALIZE_IMU,))
    PRINT_TELEMETRY = make_packet(COMMAND_PRINT_TELEMETRY, ())

if __name__ == "__main__":
    print(make_packet(COMMAND_MODE, (1,)))
