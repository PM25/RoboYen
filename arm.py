import time
from adafruit_servokit import ServoKit

kit = ServoKit(channels=16)


class Servo:
    def __init__(self):
        webcam_horizontal = 12
        webcam_vertical = 13

    def control(self):
        


kit.servo[12].angle = 0
kit.servo[12].angle = 180

#kit.continuous_servo[1].throttle = 1
#time.sleep(1)
#kit.continuous_servo[1].throttle = -1
#time.sleep(1)
#kit.servo[0].angle = 0
#kit.continuous_servo[1].throttle = 0
