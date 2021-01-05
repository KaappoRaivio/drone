COMMAND_IMU = 0
COMMAND_GET_BATTERY_VOLTAGES = 1
COMMAND_ARM_ESC = 2
COMMAND_MMANUAL_CONTROL = 3
COMMAND_DISARM_ESC = 4
COMMAND_PRINT_MODE = 5

def make_packet(command, params):
    return bytes([len(params) * 2, (command + 12) % 256, *[item for sublist in [list(param.to_bytes(2, "little")) for param in params] for item in sublist]])


if __name__ == "__main__":
    print(make_packet(12, (267, 4)))
