#include <Arduino.h>
#include "ServoMotor.h"

ServoMotor::ServoMotor(Adafruit_PWMServoDriver *pwm, int channel, int start_angle, const int rotate_speed, const int min_angle, const int max_angle, const int min_pulselen, const int max_pulselen): channel(channel), rotate_speed(rotate_speed), min_angle(min_angle), max_angle(max_angle), min_pulselen(min_pulselen), max_pulselen(max_pulselen) {
  this->pwm = pwm;
  this->curr_pulselen = this->angle_to_pulselen(start_angle);
  this->target_pulselen = this->curr_pulselen;

  delay(100);
  pwm->setPWM(this->channel, 0, this->curr_pulselen);
  delay(1000);
}

int ServoMotor::angle_to_pulselen(int angle) {
  
  return map(angle, this->min_angle, this->max_angle, this->min_pulselen, this->max_pulselen);
}

int ServoMotor::set_angle(int angle) {
  
  this->target_pulselen = this->angle_to_pulselen(angle);
}

void ServoMotor::update() {
  
  if(this->target_pulselen > this->curr_pulselen) {
    this->curr_pulselen += this->rotate_speed;
    this->pwm->setPWM(this->channel, 0, curr_pulselen);
  } else if (this->target_pulselen < this->curr_pulselen) {
    this->curr_pulselen -= this->rotate_speed;
    this->pwm->setPWM(this->channel, 0, curr_pulselen);
  } 
  
  delay(5);
}
