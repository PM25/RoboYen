import time
import evdev
from evdev import ecodes
from pathlib import Path


class Joystick:
    def __init__(self):
        joystick_device = self.select_joystick_device()
        self.joystick = self.wake_up_joystick(joystick_device)

    def select_joystick_device(self):
        print("[Joystick]")
        devices = [evdev.InputDevice(path) for path in evdev.list_devices()]

        assert len(devices) > 0

        if len(devices) > 1:
            print()
            print("=" * 5, "Available Devices for Joystick", "=" * 5)
            for idx, device in enumerate(devices):
                print(f"{idx}:\t{device.path}\t{device.name}")
            print("=" * 30)
            device_idx = eval(input(f"Select Device >> "))

        elif len(devices) == 1:
            device_idx = 0

        device = devices[device_idx].path
        print(f"*Selected device for Joystick: {device}")

        return device

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
