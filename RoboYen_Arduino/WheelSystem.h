#include <Arduino.h>
#include "EncoderMotor.h"
#ifndef WheelSystem_h

class WheelSystem {
  private:
    volatile long left_pos;
    volatile long right_pos;
    long prev_time;
    float prev_loss;
    float loss_integral;
    const EncoderMotor &left_wheel;
    const EncoderMotor &right_wheel;
    int prev_diff = 0;
    int prev_t = 0;
  
  public:
    WheelSystem(const EncoderMotor&, const EncoderMotor&);
    void go(int, int);
    void update_left_pos(int);
    void update_right_pos(int);
    void reset();
    void manual_set(int, int);
};

#endif
