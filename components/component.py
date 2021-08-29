import sys
import evdev
import serial
from time import sleep
from pathlib import Path


class Component:
    def __init__(self, device_glob, max_try=1000, time_interval=1):
        self.device_glob = device_glob
        self.max_try = int(max_try)
        self.time_interval = time_interval
        

    def safe_execute(func):
        def execute(self):
            for _ in range(self.max_try):
                try:
                    print("=> Program Start")
                    return func(self)

                except(KeyboardInterrupt, SystemExit) as e:
                    print("ok, ok, terminating now...")
                    break

                except(OSError) as e:
                    print("*Can't detect any devices")

                except:
                    print("=" * 15)
                    print("*Unexpected error:", sys.exc_info()[0])
                    raise

                sleep(self.time_interval)
            
        return execute

    
    @safe_execute
    def select_device(self):
        print("\n[Arduino]")
        devices_name = Path("/dev").glob(self.device_glob)
        devices_name = [str(n) for n in devices_name]

        assert len(devices_name) > 0

        if len(devices_name) > 1:
            print()
            print("=" * 5, "Available Devices for Arduino", "=" * 5)
            for idx, device_name in enumerate(devices_name):
                print(f"{idx}:\t{device_name}")
            print("=" * 30)
            device_idx = eval(input("Select device >>"))

        elif len(devices_name) == 1:
            device_idx = 0

        device = devices_name[device_idx]

        print(f"*Selected device for Arduino: {device}")

        return device
    
    
    @safe_execute
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
