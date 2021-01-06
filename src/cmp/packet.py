COMMAND_IMU = 0
COMMAND_GET_BATTERY_VOLTAGES = 1
COMMAND_ARM_ESC = 2
COMMAND_MANUAL_CONTROL = 3
COMMAND_DISARM_ESC = 4
COMMAND_MODE = 5
COMMAND_DEBUG_CONTROL = 6
COMMAND_PID = 7
COMMAND_RESETI = 8

POSTAMBLE = ord(".")
PREAMBLE = ord("C")

def make_packet(command, params):
    return bytes([PREAMBLE, len(params) * 2, (command + 12) % 256, *[item for sublist in [list(param.to_bytes(2, "little")) for param in params] for item in sublist], POSTAMBLE])


if __name__ == "__main__":
    print(make_packet(COMMAND_MODE, (1,)))
