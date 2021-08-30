import time
import multiprocessing
from adafruit_servokit import ServoKit


class Arm:
    def __init__(self):
        self.kit = ServoKit(channels=16)
        self.motors = {
            "base_horizontal": 0,
            "arm_vertical1": 1,
            "arm_vertical2": 2,
            "arm_vertical3": 3,
            "gripper_rotate": 4,
            "gripper": 5,
            "webcam_horizontal": 6,
            "webcam_vertical": 7
        }
        self.pos = self.reset_pos()
        self.pos_snapshot = {}
        
        self.queue = multiprocessing.Queue()
        p1 = multiprocessing.Process(target=self.run, args=(self.queue, ))
        p1.start()

    def run(self, q):
        command = "stop"
        while True:
            if(not q.empty()):
                command, value = q.get()

            if(command == "webcam_rotate"):
                self.webcam_rotate(value)
            elif(command == "webcam_vertical"):
                self.webcam_vertical(value)
            elif(command == "gripper"):
                self.gripper(value)
            elif(command == "gripper_rotate"):
                self.gripper_rotate(value)
            elif(command == "arm_rotate"):
                self.arm_rotate(value)

            time.sleep(0.03)

    def execute_command(self, command, value):
        self.queue.put([command, value])


    def lift(self, value):
        pass

    def forward(self, value):
        pass

    def gripper(self, direction):
        motor_id = self.motors["gripper"]
        self.pos["gripper"] = min(max(self.pos["gripper"] + direction, 90), 175)
        self.kit.servo[motor_id].angle = self.pos["gripper"]

    def gripper_rotate(self, direction):
        motor_id = self.motors["gripper_rotate"]
        self.pos["gripper_rotate"] = self.clap_value(self.pos["gripper_rotate"] + direction)
        self.kit.servo[motor_id].angle = self.pos["gripper_rotate"]

    def arm_rotate(self, direction):
        motor_id = self.motors["base_horizontal"]
        self.pos["base_horizontal"] = min(max(self.pos["base_horizontal"] + direction, 15), 155)
        self.kit.servo[motor_id].angle = 180 - self.pos["base_horizontal"]

    def webcam_rotate(self, direction):
        motor_id = self.motors["webcam_horizontal"]
        self.pos["webcam_horizontal"] = self.clap_value(self.pos["webcam_horizontal"] + direction)
        self.kit.servo[motor_id].angle = 180 - self.pos["webcam_horizontal"]
    
    def webcam_vertical(self, direction):
        motor_id = self.motors["webcam_vertical"]
        self.pos["webcam_vertical"] = self.clap_value(self.pos["webcam_vertical"] + direction)
        self.kit.servo[motor_id].angle = 180 - self.pos["webcam_vertical"]

    def save_pos(self):
        self.pos_snapshot = self.pos.copy()

    def reset_pos(self):
        motors_pos = {
            "base_horizontal": 70,
            "arm_vertical1": 20,
            #"arm_vertical2": 90,
            #"arm_vertical3": 90,
            "gripper_rotate": 100,
            "gripper": 90,
            "webcam_horizontal": 90,
            "webcam_vertical": 80
        }
        
        for motor_name, pos in motors_pos.items():
            motor_id = self.motors[motor_name]
            self.kit.servo[motor_id].angle = self.clap_value(pos)
            time.sleep(2)

        return motors_pos

    def hide_from_camera_pos(self):
        motors_pos = {
            "gripper_rotate": 90,
            "forward": 100,
            "lift": -100,
            "arm_rotate": 90,
        }
        
        for motor_name, pos in motors_pos:
            motor_id = self.motors[motor_name]
            self.kit.servo[motor_id].angle = self.clap_value(pos)

    def saved_pos(self):
        for motor_id, pos in self.pos_snapshot:
            self.kit.servo[motor_id].angle = self.clap_value(pos)

    def clap_value(self, value):
        return max(0, min(180, value))
       

if __name__ == "__main__":
    print('prepare')
    arm = Arm()
    print('start')

    arm.execute_command("gripper_rotate", 2)
    arm.execute_command("arm_rotate", 2)
    time.sleep(5)
    arm.execute_command("gripper_rotate", -2)
    arm.execute_command("arm_rotate", -2)
    time.sleep(5)
    arm.execute_command("gripper_rotate", 2)
    arm.execute_command("arm_rotate", 2)
