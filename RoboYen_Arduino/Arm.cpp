#include <Arduino.h>
#include "Arm.h"

Arm::Arm(int SERVO_FREQ) {
  delay(10);
  this->pwm = Adafruit_PWMServoDriver();
  this->pwm.begin();
  this->pwm.setOscillatorFrequency(27000000);
  this->pwm.setPWMFreq(SERVO_FREQ);  // Analog servos run at ~50 Hz updates
  delay(10);

  this->base_horizontal = new ServoMotor(&this->pwm, 0, 90, 2, 0, 160, 120, 450);
  this->arm_vertical1 = new ServoMotor(&this->pwm, 1, 55, 1, 55, 210, 180, 400);
  this->arm_vertical2 = new ServoMotor(&this->pwm, 2, 100, 1, 10, 200, 100, 450);
  this->arm_vertical3 = new ServoMotor(&this->pwm, 3, 10, 1, -20, 125, 170, 400);
  this->gripper_rotation = new ServoMotor(&this->pwm, 4, 0, 2, -25, 180, 90, 465);
  this->gripper = new ServoMotor(&this->pwm, 5, 20, 5, 0, 90, 285, 420);
  this->webcam_horizontal = new ServoMotor(&this->pwm, 6, 90, 5, -15, 170, 90, 470);
  this->webcam_vertical = new ServoMotor(&this->pwm, 7, 0, 5, 0, 110, 90, 460);
}

ServoMotor* Arm::id_to_motor(int id) {
  switch(id){
  case 0:
    return this->base_horizontal;
  case 1:
    return this->arm_vertical1;
  case 2:
    return this->arm_vertical2;
    break;
  case 3:
    return this->arm_vertical3;
  case 4:
    return this->gripper_rotation;
  case 5:
    return this->gripper;
  case 6:
    return this->webcam_horizontal;
  case 7:
    return this->webcam_vertical;
  }
}

void Arm::set_angle(int id, int angle) {
  this->id_to_motor(id)->set_angle(angle);
}

void Arm::set_rotate(int id, int direction) {
  this->id_to_motor(id)->set_rotate(direction);
}

void Arm::set_speed(int id, int new_speed) {
  this->id_to_motor(id)->set_speed(new_speed);
}

void Arm::lift(int distance) {
  this->arm_vertical1->set_relative_angle(2 * distance);
  this->arm_vertical2->set_relative_angle(distance);
  this->arm_vertical3->set_relative_angle(distance);
}

void Arm::update_all() {
  this->base_horizontal->update();
  this->arm_vertical1->update();
  this->arm_vertical2->update();
  this->arm_vertical3->update();
  this->gripper_rotation->update();
  this->gripper->update();
  this->webcam_horizontal->update();
  this->webcam_vertical->update();
}
