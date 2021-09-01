#include <Arduino.h>
#include "Arm.h"

Arm::Arm(int SERVO_FREQ) {
  delay(100);
  this->pwm = Adafruit_PWMServoDriver();
  this->pwm.begin();
  this->pwm.setOscillatorFrequency(27000000);
  this->pwm.setPWMFreq(SERVO_FREQ);  // Analog servos run at ~50 Hz updates
  delay(100);

  this->base_horizontal = new ServoMotor(&this->pwm, 0, 100, 7, 0, 180, 110, 460);
  this->arm_vertical1 = new ServoMotor(&this->pwm, 1, 0, 1, 0, 180, 180, 400);
  this->arm_vertical2 = new ServoMotor(&this->pwm, 2, 100, 1, 0, 180, 100, 460);
  this->arm_vertical3 = new ServoMotor(&this->pwm, 3, 30, 1, 0, 180, 170, 410);
  this->gripper_rotation = new ServoMotor(&this->pwm, 4, 27, 3, 0, 180, 80, 465);
  this->gripper = new ServoMotor(&this->pwm, 5, 20, 6, 0, 90, 285, 420);
  this->webcam_horizontal = new ServoMotor(&this->pwm, 6, 105, 10, 0, 180, 90, 470);
  this->webcam_vertical = new ServoMotor(&this->pwm, 7, 60, 10, 0, 180, 90, 470);
}

void Arm::rotate_arm(int angle) {
  this->base_horizontal->set_angle(angle);
}

void Arm::move_arm_vertical1(int angle) {
  this->arm_vertical1->set_angle(angle);
}

void Arm::move_arm_vertical2(int angle) {
  this->arm_vertical2->set_angle(angle);
}

void Arm::move_arm_vertical3(int angle) {
  this->arm_vertical3->set_angle(angle);
}

void Arm::rotate_gripper(int angle) {
  this->gripper_rotation->set_angle(angle);
}

void Arm::move_gripper(int angle) {
  this->gripper->set_angle(angle);
}

void Arm::move_webcam_horizontal(int angle) {
  this->webcam_horizontal->set_angle(angle);
}

void Arm::move_webcam_vertical(int angle) {
  this->webcam_vertical->set_angle(angle);
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
