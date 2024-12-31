#include "MotorControl.h"
#include <Arduino.h>


MotorControl::MotorControl(int detectpin,int outpin1,int outpin2,String motorname)
: _detectpin(detectpin), _outpin1(outpin1), _outpin2(outpin2), _motorname(motorname){

    pinMode(_detectpin,OUTPUT);
    pinMode(_outpin1,OUTPUT);
    pinMode(_outpin2,OUTPUT);
}

void MotorControl::forward(){
    digitalWrite(_detectpin,HIGH);
    digitalWrite(_outpin1,HIGH);
    digitalWrite(_outpin2,LOW);
}

void MotorControl::reverse(){
    digitalWrite(_detectpin,HIGH);
    digitalWrite(_outpin1,LOW);
    digitalWrite(_outpin2,HIGH);
}

void MotorControl::stop(){
    digitalWrite(_detectpin,HIGH);
    digitalWrite(_outpin1,LOW);
    digitalWrite(_outpin2,LOW);
}

void MotorControl::reset(){
    digitalWrite(_detectpin,LOW);
    digitalWrite(_outpin1,LOW);
    digitalWrite(_outpin2,LOW);
}