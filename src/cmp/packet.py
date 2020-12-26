COMMAND_ADD = 0
COMMAND_MULTIPLY = 1
COMMAND_PRINT_IMU = 2
COMMAND_ZERO_IMU = 3
COMMAND_GET_BATTERY_VOLTAGES = 4


def make_packet(command, params):
    return bytes([len(params) * 2, (command + 12) % 256, *[item for sublist in [list(param.to_bytes(2, "little")) for param in params] for item in sublist]])


if __name__ == "__main__":
    print(make_packet(12, (267, 4)))
