#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include "ServoMotor.h"
#ifndef Arm_h

class Arm {
  private:
    Adafruit_PWMServoDriver pwm;
    ServoMotor *base_horizontal,
               *arm_vertical1,
               *arm_vertical2,
               *arm_vertical3,
               *gripper_rotation,
               *gripper,
               *webcam_horizontal,
               *webcam_vertical;
  public:
    Arm(int);
    void rotate_arm(int);
    void update_all();
    void move_arm_vertical1(int);
    void move_arm_vertical2(int);
    void move_arm_vertical3(int);
    void rotate_gripper(int);
    void move_gripper(int);
    void move_webcam_vertical(int);
    void move_webcam_horizontal(int);
};

#endif
