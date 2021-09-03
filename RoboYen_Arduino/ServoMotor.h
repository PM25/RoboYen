#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#ifndef ServoMotor_h

class ServoMotor {
  private:
    const int channel;
    volatile int target_pulselen;
    volatile int curr_pulselen;
    const int min_angle;
    const int max_angle;
    const int min_pulselen;
    const int max_pulselen;
    int rotate_speed;
    Adafruit_PWMServoDriver *pwm;
  
  public:
    ServoMotor(Adafruit_PWMServoDriver*, int, int, int, int, int, int, int);
    int set_angle(int);
    int angle_to_pulselen(int);
    void set_speed(int);
    void set_rotate(int);
    void set_relative_angle(int);
    void update();
};

#endif
