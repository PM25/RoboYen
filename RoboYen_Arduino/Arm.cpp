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
  this->arm_vertical2 = new ServoMotor(&this->pwm, 2, 90, 1, 10, 200, 100, 450);
  this->arm_vertical3 = new ServoMotor(&this->pwm, 3, 35, 1, -10, 120, 170, 400);
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

void Arm::reset_pos() {
  this->base_horizontal->set_degree(90);
  this->arm_vertical1->set_degree(55);
  this->arm_vertical2->set_degree(90);
  this->arm_vertical3->set_degree(35);
  this->gripper_rotation->set_degree(0);
  this->gripper->set_degree(20);
  this->webcam_horizontal->set_degree(90);
  this->webcam_vertical->set_degree(0);
}

void Arm::set_degree(int id, float degree) {
  this->id_to_motor(id)->set_degree(degree);
}

void Arm::set_rotate(int id, int direction) {
  this->id_to_motor(id)->set_rotate(direction);
}

void Arm::set_speed(int id, int new_speed) {
  this->id_to_motor(id)->set_speed(new_speed);
}

void Arm::force_stop() {
  this->base_horizontal->force_stop();
  this->arm_vertical1->force_stop();
  this->arm_vertical2->force_stop();
  this->arm_vertical3->force_stop();
  this->gripper_rotation->force_stop();
  this->gripper->force_stop();
  this->webcam_horizontal->force_stop();
  this->webcam_vertical->force_stop();
}

void Arm::lift(int distance) {
  float deg1 = this->arm_vertical1->get_target_degree(),
        deg2 = this->arm_vertical2->get_target_degree(),
        deg3 = this->arm_vertical3->get_target_degree();
  
  float new_deg1 = deg1 + distance,
        new_deg2, new_deg3;
  
  float x = cos(radians(deg1)) + 0.2 * cos(radians(deg2 - deg1)) + 0.5 * sin(radians(deg2 - deg1)) - cos(radians(new_deg1));
  float sin_x = (x + sqrt(x * x - 1.16 * (x * x - 0.04))) / 0.58;
  float sin_x2 = (x - sqrt(x * x - 1.16 * (x * x - 0.04))) / 0.58;

  float ans1 = degrees(asin(sin_x));
  float ans2 = degrees(asin(sin_x2));
  float real_ans;

  if(ans1 < ans2) {
    if(ans1 <= 0) real_ans = ans2;
    else real_ans = ans1;
  } else {
    if(ans2 <= 0) real_ans = ans1;
    else real_ans = ans2;
  }

  new_deg2 = deg2 + real_ans;
  new_deg3 = 1 - real_ans + deg3;

  if(this->arm_vertical1->is_valid_degree(new_deg1) && this->arm_vertical2->is_valid_degree(new_deg2) && this->arm_vertical3->is_valid_degree(new_deg3)) {
    this->arm_vertical1->set_degree(new_deg1);
    this->arm_vertical2->set_degree(new_deg2);
    this->arm_vertical3->set_degree(new_deg3);    
  }
}

void Arm::front(int distance) {
  float deg1 = this->arm_vertical1->get_target_degree(),
        deg2 = this->arm_vertical2->get_target_degree(),
        deg3 = this->arm_vertical3->get_target_degree();
  
  float new_deg1 = deg1 + distance,
        new_deg2, new_deg3;
  
  float x = sin(radians(deg1)) + 0.2 * sin(radians(deg2 - deg1)) + 0.5 * cos(radians(deg2 - deg1)) - sin(radians(new_deg1));
  float cos_x = (x + sqrt(x * x - 1.16 * (x * x - 0.04))) / 0.58;
  float cos_x2 = (x - sqrt(x * x - 1.16 * (x * x - 0.04))) / 0.58;

  float ans1 = degrees(acos(cos_x));
  float ans2 = degrees(acos(cos_x2));
  float real_ans;

  if(ans1 < ans2) {
    if(ans1 <= 0) real_ans = ans2;
    else real_ans = ans1;
  } else {
    if(ans2 <= 0) real_ans = ans1;
    else real_ans = ans2;
  }

  new_deg2 = deg2 + real_ans;
  new_deg3 = 1 - real_ans + deg3;

  Serial.println(new_deg1);
  Serial.println(new_deg2);
  Serial.println(new_deg3);

  if(this->arm_vertical1->is_valid_degree(new_deg1) && this->arm_vertical2->is_valid_degree(new_deg2) && this->arm_vertical3->is_valid_degree(new_deg3)) {
    this->arm_vertical1->set_degree(new_deg1);
    this->arm_vertical2->set_degree(new_deg2);
    this->arm_vertical3->set_degree(new_deg3);    
  }
}

float Arm::get_y_distance() {
  float deg1 = this->arm_vertical1->get_curr_degree(),
        deg3 = this->arm_vertical2->get_curr_degree() - deg1,
        deg2 = 90 - deg3,
        deg4 = this->arm_vertical3->get_curr_degree() + deg3,
        deg5 = 90 - deg4;
  
  return 9*sin(radians(deg1)) + 2.5*sin(radians(deg2)) + 4.5*sin(radians(deg3)) + 2.5*sin(radians(deg4)) - 10*sin(radians(deg5));
}

float Arm::get_x_distance() {
  float deg1 = this->arm_vertical1->get_curr_degree(),
        deg3 = this->arm_vertical2->get_curr_degree() - deg1,
        deg2 = 90 - deg3,
        deg4 = this->arm_vertical3->get_curr_degree() + deg3,
        deg5 = 90 - deg4;
  
  return -9*cos(radians(deg1)) - 2.5*cos(radians(deg2)) + 4.5*cos(radians(deg3)) + 2.5*cos(radians(deg4)) + 10*cos(radians(deg5));
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
