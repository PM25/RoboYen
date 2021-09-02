#include <Arduino.h>
#include "WheelSystem.h"

WheelSystem::WheelSystem(const EncoderMotor &left_motor, const EncoderMotor &right_motor): left_wheel(left_motor), right_wheel(right_motor){
  this->left_pos = 0;
  this->right_pos = 0;
  this->prev_time = 0;
  this->prev_loss = 0;
  this->loss_integral = 0;
  this->prev_diff = 0;
  this->prev_t = 0;
  
  this->left_wheel.set_power(255);
  this->right_wheel.set_power(255);
}

void WheelSystem::go(int move_direction, int power) {
  this->left_wheel.set_max_power(power);
  this->right_wheel.set_max_power(power);
  
  this->left_wheel.rotate(move_direction);
  this->right_wheel.rotate(move_direction);
  
  int diff = abs(abs(this->left_pos) - abs(this->right_pos)); // left and right motor spin in different direction

  long curr_time = micros();
  float delta_time = ((float) (curr_time - this->prev_t)) / 1.0e6;

  int derivative = ((this->prev_diff - diff) / delta_time) * 10;
  
  if(diff > 10) {
    if(abs(left_pos) > abs(right_pos)) {
      if(this->right_wheel.get_max_power() > this->right_wheel.get_power()) {
        this->right_wheel.set_power(this->right_wheel.get_max_power());
      } else {
        this->left_wheel.set_power(this->left_wheel.get_max_power() - diff + derivative);
      }
    } else {
      if(this->left_wheel.get_max_power() > this->left_wheel.get_power()) {
        this->left_wheel.set_power(this->left_wheel.get_max_power());
      } else {
        this->right_wheel.set_power(this->right_wheel.get_max_power() - diff + derivative);
      }
    }
  } else {
    this->right_wheel.set_power(this->right_wheel.get_max_power());
    this->left_wheel.set_power(this->left_wheel.get_max_power());
  }

}

void WheelSystem::update_left_pos(int left_pos) {
  this->left_pos -= left_pos;  
}

void WheelSystem::update_right_pos(int right_pos) {
  this->right_pos -= right_pos;
}

void WheelSystem::reset() {
  this->left_pos = 0;
  this->right_pos = 0;
  this->prev_time = 0;
  this->prev_loss = 0;
  this->loss_integral = 0;
  this->prev_diff = 0;
  this->prev_t = 0;
  
  this->left_wheel.set_power(255);
  this->right_wheel.set_power(255);
}

// left_power, right_power: 0-510
void WheelSystem::manual_set(int left_power, int right_power) {
  this->left_wheel.set_max_power(255);
  this->right_wheel.set_max_power(255);

  this->left_wheel.set_power_direction(left_power - 255);
  this->right_wheel.set_power_direction(right_power - 255);
}

