#include <Arduino.h>
#include "ServoMotor.h"

ServoMotor::ServoMotor(Adafruit_PWMServoDriver *pwm, int channel, int start_degree, int rotate_speed, const int min_degree, const int max_degree, const int min_pulselen, const int max_pulselen): channel(channel), min_degree(min_degree), max_degree(max_degree), min_pulselen(min_pulselen), max_pulselen(max_pulselen) {
  this->pwm = pwm;
  this->curr_pulselen = this->degree_to_pulselen(start_degree);
  this->target_pulselen = this->curr_pulselen;
  this->rotate_speed = rotate_speed;

  delay(50);
  pwm->setPWM(this->channel, 0, this->curr_pulselen);
  delay(500);
}

int ServoMotor::is_valid_degree(float degree) {
  if(degree < this->min_degree || degree > this->max_degree) return 0;
  else return 1;
}

float ServoMotor::mapfloat(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

int ServoMotor::degree_to_pulselen(float degree) {
  return this->mapfloat(degree, this->min_degree, this->max_degree, this->min_pulselen, this->max_pulselen);
}

float ServoMotor::pulselen_to_degree(int pulselen) {
  return this->mapfloat(pulselen, this->min_pulselen, this->max_pulselen, this->min_degree, this->max_degree);
}

int ServoMotor::clap_pulselen(int pulselen) {
  return min(this->max_pulselen, max(this->min_pulselen, pulselen));
}

int ServoMotor::set_degree(float degree) {
  this->target_pulselen = this->clap_pulselen(this->degree_to_pulselen(degree));
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

void ServoMotor::set_relative_degree(float degree) {
  int relative_pulse = degree * (float)(this->max_pulselen - this->min_pulselen) / (this->max_degree - this->min_degree);
  this->target_pulselen += relative_pulse;
}

float ServoMotor::get_curr_degree() {
  return this->pulselen_to_degree(this->curr_pulselen);
}

float ServoMotor::get_target_degree() {
  return this->pulselen_to_degree(this->target_pulselen);
}

void ServoMotor::force_stop() {
  this->target_pulselen = this->curr_pulselen;
//  this->pwm->setPin(this->channel, 0);
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
