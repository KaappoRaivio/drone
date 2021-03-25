import pygame
import listener
import xbox360
import packet

import math,time
#
# pygame.joystick.init()
pygame.init()


class KeyboardInputSupplier:
    _forward_keys = {
        "w": 1000,
        "s": -1000
    }

    _turning_keys = {
        "a": 80,
        "d": -80
    }

    _grabber_keys = {
        "r": -999,
        "f": 999
    }

    def __init__(self):
        self.listener = listener.KeyboardListener()

        self.__hold_key_pressed_before = False

    def getInput(self):
        forward_speed = 0
        turning_speed = 0
        grabber_speed = 0

        for key in self.listener.keys_down:
            forward_speed += self._forward_keys.get(key, 0)
            turning_speed += self._turning_keys.get(key, 0)
            # grabber_speed += self._grabber_keys.get(key, 0)

        return f"MOVE {forward_speed} {forward_speed} 0 0 1 1 -1 1 {turning_speed} 200"

    def __getHold(self):
        hold = 0
        state = "v" in self.listener.keys_down

        if state and not self.__state_before_Y:
            hold = 1

        self.__state_before_Y = state
        return hold


class ControllerInputSupplier:
    def __init__(self, id):
        self.id = id
        self.controller = xbox360.Controller(id)

        

        self.__state_before_buttons = (False, False, False, False)  
        self.__state_before_pad = (False, False, False, False)

        self.first_time = True

        self.collective = 0

    @staticmethod
    def length(radians):
        radians %= math.pi / 2
        return 1 / math.cos(radians if radians < math.pi / 4 else math.pi / 2 - radians)

    def transform_stick(self, joystick):
        old_x, old_y = joystick
        old_y *= -1

        if old_x == 0:
            angle = 90
        else:
            angle = math.degrees(math.atan(abs(old_y / old_x)))

        if old_x <= 0 and old_y > 0:
            angle = 180 - angle
        elif old_x <= 0 and old_y <= 0:
            angle = 180 + angle
        elif old_x > 0 and old_y <= 0:
            angle = 360 - angle
        else:
            angle = angle
        angle = math.radians(angle)

        length = math.sqrt(old_x ** 2 + old_y ** 2)
        desired_length_multiplier = ControllerInputSupplier.length(angle)

        length *= desired_length_multiplier

        # print(f"{length * math.cos(angle):5.2f}, {length * math.sin(angle):5.2f},    {old_x:8.2f}, {old_y:5.2f}, {math.degrees(angle):8.2f}, {desired_length_multiplier:8.2f}")
        #print(f"{math.degrees(angle):8.2f}, {old_x:6.2f}, {old_y:6.2f}")
        return length * math.cos(angle), length * math.sin(angle)

    def getButtonsDown(self):
        res = self.controller.get_buttons()

        res = [res[i] and not self.__state_before_buttons[i] for i in range(4)]
        self.__state_before_buttons = res
        try:
            return res.index(True)
        except:
            return -1

    output = [
        (0, 50, 50, 0),
        (0, -50, 50, 0),
    ]

    def getInput(self, debug=False):
        pygame.event.pump()

        # if self.first_time:
        #     self.first_time = False
        #     return "CONNECT 192.168.43.109 65533"

        roll, pitch, = self.transform_stick(self.controller.get_left_stick())
        yaw, _ = self.transform_stick(self.controller.get_right_stick())

        self.collective += self.controller.get_triggers() * 10

        print(f"{self.collective:6.2f} {pitch:6.2f} {roll:6.2f} {yaw:6.2f}")

        scale = 750

        down = self.getButtonsDown()
        
        if down == 0:
            
            while len((new_pid := input("Enter new pid values: [axis] [p] [i] [d]").split(" "))) != 4:
                pass
            
            return packet.make_packet(packet.COMMAND_PID, tuple(map(int, new_pid)))
        elif down == 1:
            return packet.make_packet(packet.COMMAND_RESETI, ())


        p = packet.make_packet(packet.COMMAND_MANUAL_CONTROL, (scale, *tuple(map(lambda x: min(2 * scale, max(0, int(
            x * scale + scale))), (self.collective / scale * 0.8 + 0.2, pitch / scale * 30, roll / scale * 30, yaw / scale * 200,)))))

        if down == 2:
            p += packet.make_packet(packet.COMMAND_GET_BATTERY_VOLTAGES, ())

        return p

    def stop(self):
        return False


def getSupplier():
    return ControllerInputSupplier(0)

if __name__ == "__main__":
    supplier = getSupplier()

    while True:
        print(supplier.getInput(True))
        time.sleep(0.02)
