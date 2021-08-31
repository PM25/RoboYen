#include <Arduino.h>
#include "EncoderMotor.h"

EncoderMotor::EncoderMotor(const AF_DCMotor &motor, float k_loss, float k_der, float k_int, int reverse, int max_power, int min_power): my_motor(motor) {
  this->prev_time = 0;
  this->prev_loss = 0;
  this->loss_integral = 0;
  this->k_loss = k_loss;
  this->k_derivative = k_der;
  this->k_integral = k_int;
  this->reverse = reverse;
  this->max_power = max_power;
  this->min_power = min_power;
  this->power = 0;
  
  
  if(reverse) {
    this->forward = BACKWARD;
    this->backward = FORWARD;
  } else {
    this->forward = FORWARD;
    this->backward = BACKWARD;
  }
}

void EncoderMotor::set_pos(int curr_pos, int target_pos) {
  if(this->reverse) target_pos *= -1;
  
  long curr_time = micros();
  float delta_time = ((float) (curr_time - this->prev_time)) / 1.0e6;

  int loss = target_pos - curr_pos;
  
  float derivative = (loss - this->prev_loss) / delta_time;
  this->loss_integral += loss * delta_time;
  
  float u = this->k_loss * loss + this->k_derivative * derivative + this->k_integral * this->loss_integral;
  
  float power = fabs(u);
  if(power > this->max_power) {
    power = this->max_power;
  }
  this->set_power(power);

  if(power < 100 || abs(loss) < 10) {
    this->my_motor.run(RELEASE);
  } else if(u < 0) {
    this->my_motor.run(this->backward);
  } else {
    this->my_motor.run(this->forward);
  }

  this->prev_loss = loss;
  this->prev_time = curr_time;
  this->prev_pos = curr_pos;

  Serial.print(loss);
  Serial.print(", ");
  Serial.println(u);
}

void EncoderMotor::rotate(int direct) { 
  if(direct > 0) {
    this->my_motor.run(this->forward);
  } else if(direct < 0) {
    this->my_motor.run(this->backward);
  } else if(direct == 0) {
    this->my_motor.run(RELEASE);
  }
}

void EncoderMotor::set_power(int power) {
  if(power > this->max_power) {
    power = this-> max_power;
  } else if(power < this->min_power) {
    this->my_motor.run(RELEASE);
    power = this->min_power;
  }
  
  this->power = power;
  this->my_motor.setSpeed(this->power);
}

// power (int): -255 ~ 255
void EncoderMotor::set_power_direction(int power) {
  this->rotate(power);
  this->set_power(abs(power));
}

void EncoderMotor::set_max_power(int power) {
  if(power > 255) {
    power = 255;
  } else if(power < 0) {
    power = 0;
  }
  
  this->max_power = power;
}

int EncoderMotor::get_power() {
  return this->power;
}

int EncoderMotor::get_max_power() {
  return this->max_power;
}
