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
    void reset_pos();
    void rotate_arm(int);
    void update_all();
    void set_degree(int, float);
    void set_rotate(int, int);
    void set_speed(int, int);
    void lift(int);
    void front(int);
    void force_stop();
    float get_x_distance();
    float get_y_distance();
    ServoMotor* id_to_motor(int);
};

#endif
