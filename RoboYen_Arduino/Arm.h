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
    void set_angle(int, int);
    void set_rotate(int, int);
    void set_speed(int, int);
    void lift(int);
    ServoMotor* id_to_motor(int);
};

#endif
