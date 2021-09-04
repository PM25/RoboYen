#include <AFMotor.h>

#include "Arm.h"
#include "WheelSystem.h"

#define Left_Encoder_Output_A 18 // pin2 of the Arduino
#define Left_Encoder_Output_B 22 // pin3 of the Arduino
#define Right_Encoder_Output_A 19 // pin4 of the Arduino
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

EncoderMotor left_wheel(left_motor, 1.0, 0.166, 0, 1, 255, 70);
EncoderMotor right_wheel(right_motor, 1.0, 0.05, 0, 0, 255, 70);

WheelSystem robot(left_wheel, right_wheel);

Arm *arm;

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

  delay(50);
  arm = new Arm(50);
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
    command = "NUL";
  } else if(!strcmp(command, "LGF")) {
    if(value1) digitalWrite(Car_LED_Pin, LOW);
    else digitalWrite(Car_LED_Pin, HIGH);
    command = "NUL";
  } else if(!strcmp(command, "SVA")) {
    arm->set_degree(value1, value2);
    command = "NUL";
  } else if(!strcmp(command, "SVR")) {
    arm->set_rotate(value1, value2);
    command = "NUL";
  } else if(!strcmp(command, "SVS")) {
    arm->set_speed(value1, value2);
    command = "NUL";
  } else if(!strcmp(command, "GTX")) {
    Serial.print("x distance: ");
    Serial.println(arm->get_x_distance());
    command = "NUL";
  } else if(!strcmp(command, "GTY")) {
    Serial.print("y distance: ");
    Serial.println(arm->get_y_distance());
    command = "NUL";
  } else if(!strcmp(command, "LFT")) {
    arm->lift(value1);
  } else if(!strcmp(command, "FRT")) {
    arm->front(value1);
  } else if(!strcmp(command, "RST")) {
    arm->reset_pos();
    robot.reset();
    command = "NUL";
  } else if(!strcmp(command, "KLL")) {
    robot.go(0, 0);
    arm->force_stop();
  }

  arm->update_all();
  
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
