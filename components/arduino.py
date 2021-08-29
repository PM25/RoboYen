import time
import serial
from pathlib import Path

from .component import Component


class Arduino(Component):
    def __init__(self, try_times=3):
        super().__init__("ttyUSB*")
        self.arduino_device = self.select_device()
        self.arduino = serial.Serial(self.arduino_device, 9600, timeout=1)
        self.arduino.flush()
        
        self.try_times = try_times

        self.STOP = 0
        self.FORWARD = 1
        self.BACKWARD = 2
        self.LEFT = 3
        self.RIGHT = 4
        self.INCREASE_SPEED = 5
        self.DECREASE_SPEED = 6

    def send_command(self, command, value1=0, value2=0):
        command = f"{command},{int(value1):03},{int(value2):03}"

        send_time = time.time()
        for _ in range(self.try_times):
            print(f"*Send Data: {command}")
            self.arduino.write(f"{command}\n".encode())

            try:
                data = self.arduino.readline().decode().strip()
                print("*Receive Data:", data)
                if data == "ACK":
                    print(f"Pass time: {time.time() - send_time} seconds\n")
                    break
            except:
                self.arduino.close()
