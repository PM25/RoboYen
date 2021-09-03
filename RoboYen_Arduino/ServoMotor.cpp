#include <Arduino.h>
#include "ServoMotor.h"

ServoMotor::ServoMotor(Adafruit_PWMServoDriver *pwm, int channel, int start_angle, int rotate_speed, const int min_angle, const int max_angle, const int min_pulselen, const int max_pulselen): channel(channel), min_angle(min_angle), max_angle(max_angle), min_pulselen(min_pulselen), max_pulselen(max_pulselen) {
  this->pwm = pwm;
  this->curr_pulselen = this->angle_to_pulselen(start_angle);
  this->target_pulselen = this->curr_pulselen;
  this->rotate_speed = rotate_speed;

  delay(50);
  pwm->setPWM(this->channel, 0, this->curr_pulselen);
  delay(500);
}

int ServoMotor::angle_to_pulselen(int angle) {
  
  return map(angle, this->min_angle, this->max_angle, this->min_pulselen, this->max_pulselen);
}

int ServoMotor::set_angle(int angle) {
  
  this->target_pulselen = this->angle_to_pulselen(angle);
}

void ServoMotor::set_speed(int new_speed) {
  this->rotate_speed = new_speed;
}

void ServoMotor::set_rotate(int direction) {
  if(direction > 0) {
    this->target_pulselen = this->max_pulselen;
  } else if(direction < 0) {
    this->target_pulselen = this->min_pulselen;
  } else {
    this->target_pulselen = this->curr_pulselen;
  }
}

void ServoMotor::set_relative_angle(int angle) {
  int relative_pulse = angle * (float)(this->max_pulselen - this->min_pulselen) / (this->max_angle - this->min_angle);
  this->target_pulselen += relative_pulse;
}

void ServoMotor::update() {
  if(abs(this->target_pulselen - this->curr_pulselen) <= this->rotate_speed) {
    this->curr_pulselen += this->target_pulselen - this->curr_pulselen;
    this->pwm->setPWM(this->channel, 0, this->curr_pulselen);
  } else if(this->target_pulselen > this->curr_pulselen) {
    this->curr_pulselen += this->rotate_speed;
    this->pwm->setPWM(this->channel, 0, this->curr_pulselen);
  } else if (this->target_pulselen < this->curr_pulselen) {
    this->curr_pulselen -= this->rotate_speed;
    this->pwm->setPWM(this->channel, 0, this->curr_pulselen);
  } else {
    this->pwm->setPin(this->channel, 0);
  }
}
