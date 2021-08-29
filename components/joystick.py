import time
import evdev
from evdev import ecodes
from pathlib import Path

from .component import Component


class Joystick(Component):
    def __init__(self):
        super().__init__(None)
        joystick_device = self.select_joystick_device()
        self.joystick = self.wake_up_joystick(joystick_device)


    # Trying to get signal from joystick until it success.
    def wake_up_joystick(self, device, time_interval=1):
        while True:
            try:
                print("\nTrying to read input from Joystick (Press any Button)")
                joystick = evdev.InputDevice(device)
                for event in joystick.read_loop():
                    print("*Sucessfully read from Joystick")
                    break
                break
            except OSError:
                print("*Failed to read from Joystick")

            time.sleep(time_interval)

        return joystick

    def get_joystick(self):
        return self.joystick
