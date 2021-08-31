#include <AFMotor.h>
#include "EncoderMotor.h"

#define Left_Encoder_Output_A 20 // pin2 of the Arduino
#define Left_Encoder_Output_B 22 // pin3 of the Arduino
#define Right_Encoder_Output_A 21 // pin4 of the Arduino
#define Right_Encoder_Output_B 23 // pin5 of the Arduino

#define Red_Pin 24
#define Green_Pin 25
#define Blue_Pin 26
#define Buzzer_Pin 27
#define Car_LED_Pin 28
#define Arm_LED_Pin 29

#define Serial_Input_Size 12

AF_DCMotor left_motor(1, MOTOR12_8KHZ);
AF_DCMotor right_motor(2, MOTOR12_8KHZ);

class MyRobot {
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
    MyRobot(const EncoderMotor&, const EncoderMotor&);
    void go(int, int);
    void update_left_pos(int);
    void update_right_pos(int);
    void reset();
    void manual_set(int, int);
};

MyRobot::MyRobot(const EncoderMotor &left_motor, const EncoderMotor &right_motor): left_wheel(left_motor), right_wheel(right_motor){
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

void MyRobot::go(int move_direction, int power) {
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

//  Serial.print(this->left_pos);
//  Serial.print(", ");
//  Serial.print(this->right_pos);
//  Serial.print(", ");
//  Serial.print(abs(this->right_pos)-abs(this->left_pos));
//  Serial.print(", ");
//  Serial.print(this->right_wheel.get_power());
//  Serial.print(", ");
//  Serial.println(this->left_wheel.get_power());
  
//  Serial.println(diff);
}

void MyRobot::update_left_pos(int left_pos) {
  this->left_pos -= left_pos;  
}

void MyRobot::update_right_pos(int right_pos) {
  this->right_pos -= right_pos;
}

void MyRobot::reset() {
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
void MyRobot::manual_set(int left_power, int right_power) {
  this->left_wheel.set_max_power(255);
  this->right_wheel.set_max_power(255);

  this->left_wheel.set_power_direction(left_power - 255);
  this->right_wheel.set_power_direction(right_power - 255);
}

EncoderMotor left_wheel(left_motor, 1.0, 0.166, 0, 1, 255, 50);
EncoderMotor right_wheel(right_motor, 1.0, 0.05, 0, 0, 255, 70);
MyRobot robot(left_wheel, right_wheel);

void setup() {
  Serial.begin(9600); // activates the serial communication
  pinMode(Left_Encoder_Output_A, INPUT); 
  pinMode(Left_Encoder_Output_B, INPUT); 
  pinMode(Right_Encoder_Output_A, INPUT); 
  pinMode(Right_Encoder_Output_B, INPUT); 
  attachInterrupt(digitalPinToInterrupt(Left_Encoder_Output_A), Left_DC_Motor_Encoder, RISING);
  attachInterrupt(digitalPinToInterrupt(Right_Encoder_Output_A), Right_DC_Motor_Encoder, RISING);

  pinMode(Red_Pin, OUTPUT);
  pinMode(Green_Pin, OUTPUT);
  pinMode(Blue_Pin, OUTPUT);
  pinMode(Buzzer_Pin, OUTPUT);
  pinMode(Car_LED_Pin, OUTPUT);
  pinMode(Arm_LED_Pin, OUTPUT);

  digitalWrite(Red_Pin, HIGH);
  digitalWrite(Green_Pin, LOW);
  digitalWrite(Blue_Pin, LOW);
  digitalWrite(Car_LED_Pin, HIGH);
  digitalWrite(Arm_LED_Pin, HIGH);
}

char *command;
int value1(0), value2(0);
char input_data[Serial_Input_Size + 1];

void loop() {
  int start_time = millis();
  
  if(Serial.available() > 0) {
    Serial.println("ACK");
    byte input_size = Serial.readBytesUntil('\n', input_data, Serial_Input_Size);
    input_data[input_size] = 0;

    command = strtok(input_data, ",");
    value1 = atoi(strtok(0, ","));
    value2 = atoi(strtok(0, ","));
    
    if(!strcmp(command, "FWD") || !strcmp(command, "STP") || !strcmp(command, "BCK")) {
      robot.reset();
    }
  }

  if(!strcmp(command, "STP")) {
    robot.go(0, value1);
  } else if(!strcmp(command, "FWD")) {
    robot.go(1, value1);
  } else if(!strcmp(command, "BCK")) {
    robot.go(-1, value1);
  } else if(!strcmp(command, "MAN")) {
    robot.manual_set(value1, value2);
  } else if(!strcmp(command, "RDY")) {
    tone(Buzzer_Pin, 1000);
    digitalWrite(Red_Pin, LOW);
    digitalWrite(Green_Pin, HIGH);
    digitalWrite(Blue_Pin, LOW);
    delay(500);
    noTone(Buzzer_Pin);
    command = "STP";
  } else if(!strcmp(command, "PRE")) {
    digitalWrite(Red_Pin, HIGH);
    digitalWrite(Green_Pin, HIGH);
    digitalWrite(Blue_Pin, LOW);
    command = "STP";
  } else if(!strcmp(command, "LGA")) {
    if(value1) digitalWrite(Arm_LED_Pin, LOW);
    else digitalWrite(Arm_LED_Pin, HIGH);
    command = "STP";
  } else if(!strcmp(command, "LGF")) {
    if(value1) digitalWrite(Car_LED_Pin, LOW);
    else digitalWrite(Car_LED_Pin, HIGH);
    command = "STP";
  } else {
    robot.go(0, 0);
  }
  
  delay(10);
}

void Left_DC_Motor_Encoder() {
  int b = digitalRead(Left_Encoder_Output_B);
  if (b > 0) {
//    ++left_pos;
    robot.update_left_pos(1);
  }
  else {
//    --left_pos;
    robot.update_left_pos(-1);
  }
}

void Right_DC_Motor_Encoder() {
  int b = digitalRead(Right_Encoder_Output_B);
  if (b > 0) {
//    ++right_pos;
    robot.update_right_pos(1);
  }
  else {
//    --right_pos;
    robot.update_right_pos(-1);
  }
}
