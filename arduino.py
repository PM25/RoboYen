import time
import serial 
from pathlib import Path


class Arduino:
    def __init__(self, try_times=3):
        arduino_device = self.select_arduino_device()
        self.arduino = serial.Serial(arduino_device, 9600, timeout=1)
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
                if(data == "ACK"):
                    print(f"Pass time: {time.time() - send_time} seconds\n")
                    break
            except:
                self.arduino.close()
    
    def select_arduino_device(self):
        print('\n[Arduino]')
        devices_name = Path("/dev").glob("ttyUSB*")
        devices_name = [str(n) for n in devices_name]

        assert(len(devices_name) > 0)

        if(len(devices_name) > 1):
            print()
            print('=' * 5, "Available Devices for Arduino", '=' * 5)
            for idx, device_name in enumerate(devices_name):
                print(f"{idx}:\t{device_name}")
            print('=' * 30)
            device_idx = eval(input("Select device >>"))

        elif(len(devices_name) == 1):
            device_idx = 0
    
        device = devices_name[device_idx]

        print(f"*Selected device for Arduino: {device}")
    
        return device
