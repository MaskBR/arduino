#include <Arduino.h>
#include "MotorControl.h"

MotorControl motor1(2,8,9,"新风风门电机");
MotorControl motor2(3,10,11,"除霜风门电机");
MotorControl motor3(4,12,13,"右温度电机");
MotorControl motor4(5,14,15,"左温度电机");
MotorControl motor5(6,19,18,"前模式电机");
MotorControl motor6(7,17,16,"后模式电机");

int motor_num = 0;
int MotorMode = 0;
int lastMotorNum=0;
const int button1=26;//motor_num
const int button2=27;//motor_mode
int buttonState1=0;
int buttonState2=0;

MotorControl& selected_motor(int motor_num){
 switch (motor_num)
 {
  case 1:return motor1;
  case 2:return motor2;
  case 3:return motor3;
  case 4:return motor4;
  case 5:return motor5;
  case 6:return motor6;
  default:return motor1;
 }
}

void setup() {
  // 初始化串口
  Serial.begin(9600);
  // 初始化GPIO引脚
  Serial1.begin(9600);
  pinMode(button1,INPUT);
  pinMode(button2,INPUT);
  motor1.reset();
  motor2.reset();
  motor3.reset();
  motor4.reset();
  motor5.reset();
  motor6.reset();
  Serial.print("已初始化所有电机控制引脚");
}

void loop() {
  if (Serial1.available() > 0) {
    String data = Serial1.readStringUntil('\n'); // 读取一行数据
    int commaIndex = data.indexOf(','); // 查找逗号位置

    if (commaIndex > 0) {
      motor_num = data.substring(0, commaIndex).toInt(); // 提取并转换第一个变量
      MotorMode = data.substring(commaIndex + 1).toInt(); // 提取并转换第二个变量
      Serial.print("Received MotorSelect: ");
      Serial.println(motor_num);
      Serial.print("Received MotorMode: ");
      Serial.println(MotorMode);
      if (motor_num != lastMotorNum){
        MotorControl& lastMotorSelect = selected_motor(lastMotorNum);
        pin_control(lastMotorSelect, 0);
        lastMotorNum=motor_num;
      }
      MotorControl& MotorSelect = selected_motor(motor_num);
      pin_control(MotorSelect,MotorMode);
    }
  }
  buttonState1=digitalRead(button1);
  buttonState2=digitalRead(button2);
  if (buttonState1==HIGH) {
    if (lastMotorNum<6){
      motor_num=lastMotorNum+1;
    }else{
      motor_num=1;
    }
    MotorControl& MotorSelect = selected_motor(motor_num);
    MotorMode=0;
    pin_control(MotorSelect,MotorMode);
    lastMotorNum=motor_num;
    delay(500);
  }
  if (buttonState2==HIGH) {
    if (MotorMode<3){
      MotorMode+=1;
    }else{
      MotorMode=0;
    }
    MotorControl& MotorSelect = selected_motor(motor_num);
    pin_control(MotorSelect,MotorMode);
    delay(500);
  }
}

void pin_control(MotorControl& MotorSelect, int MotorMode) {
  switch (MotorMode) {
    case 1:
      MotorSelect.forward();
      Serial.print(MotorSelect.getMotorName());
      Serial.println("正转");
      break;
    case 2:
      MotorSelect.reverse();
      Serial.print(MotorSelect.getMotorName());
      Serial.println("反转");
      break;
    case 3:
      MotorSelect.stop();
      Serial.print(MotorSelect.getMotorName());
      Serial.println("停止");
      break;
    default:
      MotorSelect.reset();
      Serial.print(MotorSelect.getMotorName());
      Serial.println("重置");
      break;
  }
}