import time
from adafruit_servokit import ServoKit


class Arm:
    def __init__(self):
        self.kit = ServoKit(channels=16)
        self.motors = {
            "base_horizontal": 1,
            "arm_vertical1": 2,
            "arm_vertical2": 3,
            "arm_vertical3": 4,
            "gripper_rotate": 5,
            "gripper": 6,
            "webcam_vertical": 7,
            "webcam_horizontal": 8
        }
        self.pos = {}
        self.pos_snapshot = {}

    def lift(self, value):
        pass

    def forward(self, value):
        pass

    def gripper(self, value):
        motor_id = self.motors["gripper"]
        self.kit.servo[motor_id].angle = clap_value(value)
        self.pos[motor_id] = value

    def gripper_rotate(self, value):
        motor_id = self.motors["gripper_ratate"]
        self.kit.servo[motor_id].angle = clap_value(value)
        self.pos[motor_id] = value

    def arm_rotate(self, value):
        motor_id = self.motors["base_horizontal"]
        self.kit.servo[motor_id].angle = clap_value(value)
        self.pos[motor_id] = value

    def webcam_rotate(self, value):
        motor_id = self.motors["webcam_horizontal"]
        self.kit.servo[motor_id].angle = clap_value(value)
        self.pos[motor_id] = value
    
    def webcam_vertical(self, value):
        motor_id = self.motors["webcam_vertical"]
        self.kit.servo[motor_id].angle = clap_value(value)
        self.pos[motor_id] = value

    def save_pos(self):
        self.pos_snapshot = self.pos.copy()

    def reset_pos(self):
        motors_pos = {
            "gripper": 160,
            "gripper_rotate": 90,
            "forward": 100,
            "lift": 100,
            "arm_rotate": 90,
            "webcam_rotate": 90,
            "webcam_vertical": 50,
        }
        
        for motor_name, pos in motors_pos:
            motor_id = self.motors[motor_name]
            self.kit.servo[motor_id].angle = clap_value(pos)

    def hide_from_camera_pos(self):
        motors_pos = {
            "gripper_rotate": 90,
            "forward": 100,
            "lift": -100,
            "arm_rotate": 90,
        }
        
        for motor_name, pos in motors_pos:
            motor_id = self.motors[motor_name]
            self.kit.servo[motor_id].angle = clap_value(pos)

    def saved_pos(self):
        for motor_id, pos in self.pos_snapshot:
            self.kit.servo[motor_id].angle = clap_value(pos)

    def clap_value(self, value, reverse=False):
        value = max(0, min(180, value))
        if(reverse):
            value = 180 - value
        
        return value
