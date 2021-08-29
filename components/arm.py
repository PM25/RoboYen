import time
from adafruit_servokit import ServoKit


class Arm:
    def __init__(self):
        self.kit = ServoKit(channels=16)
        self._base_horizontal = 1
        self._arm_vertical1 = 2
        self._arm_vertical2 = 3
        self._arm_vertical3 = 4
        self._gripper_rotate = 5
        self._gripper = 6
        self._webcame_vertical = 7
        self._webcame_horizontal = 8

    def lift(self, value):
        pass

    def drop(self, value):
        pass

    def forward(self, value):
        pass

    def backward(self, value):
        pass

    def gripper(self, value):
        self.kit.servo[self.gripper_rotate].angle = clap_value(value)

    def rotate(self, value):
        self.kit.servo[self.base_horizontal].angle = clap_value(value)

    def clap_value(self, value, reverse=False):
        value = max(0, min(180, value))
        if(reverse):
            value = 180 - value
        
        return value


# kit.servo[12].angle = 0
# kit.servo[12].angle = 180

# kit.continuous_servo[1].throttle = 1
# time.sleep(1)
# kit.continuous_servo[1].throttle = -1
# time.sleep(1)
# kit.servo[0].angle = 0
# kit.continuous_servo[1].throttle = 0
