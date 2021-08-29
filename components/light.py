from .component import Component

class Light(Component):
    def __init__(self, arduino):
        self.arduino = arduino
        self.lights = {
            "front": 0,
            "arm_base": 1,
            "back": 2
        }
        self.status = {}

    def turn_all_off(self):
        pass

    def turn_all_on(self):
        pass

    def toggle_front(self):
        light_id = self.lights["front"]
        self.status[light_id] = 255 - self.status[light_id]
        self.arduino.send_command("lgf", self.status[light_id])
    
    def toggle_back(self):
        light_id = self.lights["back"]
        self.status[light_id] = 255 - self.status[light_id]
        self.arduino.send_command("lgb", self.status[light_id])

    def toggle_arm_base(self):
        light_id = self.lights["arm_base"]
        self.status[light_id] = 255 - self.status[light_id]
        self.arduino.send_command("lga", self.status[light_id])



