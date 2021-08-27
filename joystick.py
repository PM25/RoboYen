import time
import evdev
from evdev import ecodes
import serial 
from pathlib import Path


def select_arduino_device():
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



def select_joystick_device():
    devices = [evdev.InputDevice(path) for path in evdev.list_devices()]

    assert(len(devices) > 0)
    
    if(len(devices) > 1):
        print()
        print('=' * 5, "Available Devices for Joystick", '=' * 5)
        for idx, device in enumerate(devices):
            print(f"{idx}:\t{device.path}\t{device.name}")
        print('=' * 30)
        device_idx = eval(input(f"Select Device >> "))

    elif(len(devices) == 1):
        device_idx = 0

    device = devices[device_idx].path
    print(f"*Selected device for Joystick: {device}")
    
    return device




class Robot:
    def __init__(self, arduino_device, try_times=3):
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


class ControlSystem:
    def __init__(self, robot, joystick):
        self.robot = robot
        self.joystick = joystick
        
        self.BTN_A = 304 
        self.BTN_B = 305
        self.BTN_X = 307
        self.BTN_Y = 308
        
        self.abs_x = 0
        self.abs_y = 0

    def abs_x_y_to_wheel_power(self, abs_x=None, abs_y=None):
        if(abs_x is not None):
            self.abs_x = (abs_x - 128) * 2
            if(self.abs_x != 0):
                self.abs_x += 1
        if(abs_y is not None):
            self.abs_y = -(abs_y - 128) * 2
            if(self.abs_y != 0):
                self.abs_y -= 1
        
        abs_x = abs(self.abs_x)
        abs_y = abs(self.abs_y)
        
        if(abs_x + abs_y > 0):
            max_power = max(abs_x, abs_y)
            other_power = ((abs_y - abs_x) / (abs_x + abs_y)) * max_power
            other_power = round(other_power)
        else:
            max_power = 0
            other_power = 0


        if(self.abs_x > 0):
            if(self.abs_y > 0):
                left_power, right_power = max_power, other_power
            else:
                left_power, right_power = -other_power, -max_power
        else:
            if(self.abs_y > 0):
                left_power, right_power = other_power, max_power
            else:
                left_power, right_power = -max_power, -other_power

        '''
        if(self.abs_x > 0):
            left_wheel = self.abs_y
            if(self.abs_y > 0):
                right_wheel = max(0, self.abs_y - self.abs_x)
            else:
                right_wheel = self.abs_y + self.abs_x
        else:
            right_wheel = self.abs_y
            if(self.abs_y > 0):
                left_wheel = max(0, self.abs_y + self.abs_x)
            else:
                left_wheel = self.abs_y - self.abs_x
        
        print(left_wheel + 255, right_wheel + 255)
        return left_wheel + 255, right_wheel + 255
        '''
        
        return left_power + 255, right_power + 255
        
    def start(self):
        for event in self.joystick.read_loop():
            if event.type == evdev.ecodes.EV_KEY:
                keyevent = evdev.categorize(event)
                # print(keyevent)
                if keyevent.keystate == evdev.KeyEvent.key_down:
                    if keyevent.scancode == self.BTN_A:
                        self.robot.send_command("BCK", 255)
                    elif keyevent.scancode == self.BTN_B:
                        self.robot.send_command("STP")
                    elif keyevent.scancode == self.BTN_X:
                        self.robot.send_command("STP")
                    elif keyevent.scancode == self.BTN_Y:
                        self.robot.send_command("FWD", 255)
            
            elif event.type == ecodes.EV_ABS:
                absevent = evdev.categorize(event)
                
                btn = ecodes.bytype[absevent.event.type][absevent.event.code]
                value = absevent.event.value

                left_power, right_power = 0, 0

                if(btn == "ABS_Y"):
                    left_power, right_power = self.abs_x_y_to_wheel_power(abs_y=value)
                
                elif(btn == "ABS_X"):
                    left_power, right_power = self.abs_x_y_to_wheel_power(abs_x=value)

                self.robot.send_command("MAN", left_power, right_power)


# Trying to get signal from joystick until it success.
def wake_up_joystick(device):
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
        
        time.sleep(1)
    
    return joystick


def main():
    print("[Arduino]")
    
    arduino_device = select_arduino_device()
    robot = Robot(arduino_device)
    
    print("[Joystick]")
    joystick_device = select_joystick_device()
    joystick = wake_up_joystick(joystick_device)

    time.sleep(1)
    
    print("\n[Start]")
    control = ControlSystem(robot, joystick)
    control.start()
    

if __name__ == "__main__":
    while True:
        try:
            print("=> Trying to execute main()")
            main()
        
        except(KeyboardInterrupt, SystemExit) as e :
            print('ok, ok, terminating now...')
            break
        
        except:
            print("*Failed to execute main()")
        
        time.sleep(1)
