#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#ifndef ServoMotor_h

class ServoMotor {
  private:
    const int channel;
    volatile int target_pulselen;
    volatile int curr_pulselen;
    const int min_degree;
    const int max_degree;
    const int min_pulselen;
    const int max_pulselen;
    int rotate_speed;
    Adafruit_PWMServoDriver *pwm;
  
  public:
    ServoMotor(Adafruit_PWMServoDriver*, int, int, int, int, int, int, int);
    float mapfloat(float, float, float, float, float);
    int set_degree(float);
    int degree_to_pulselen(float);
    float pulselen_to_degree(int);
    void set_speed(int);
    void set_rotate(int);
    void set_relative_degree(float);
    float get_curr_degree();
    float get_target_degree();
    int clap_pulselen(int);
    int is_valid_degree(float);
    void force_stop();
    void update();
};

#endif
