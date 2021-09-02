#include <Arduino.h>
#include <AFMotor.h>
#ifndef EncoderMotor_h

class EncoderMotor {
  private:
    const AF_DCMotor &my_motor;
    long prev_time;
    int prev_pos;
    float prev_loss;
    float loss_integral;
    float k_loss;
    float k_derivative;
    float k_integral;
    int forward;
    int backward;
    int reverse;
    int max_power;
    int min_power;
    volatile int power;
  
  public:
    EncoderMotor(const AF_DCMotor&, float, float, float, int, int, int);
    void set_pos(int, int);
    void rotate(int);
    void set_power(int);
    void set_max_power(int);
    int get_power();
    int get_max_power();
    void set_power_direction(int);
};

#endif
