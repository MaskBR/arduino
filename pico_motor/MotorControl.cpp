#include "MotorControl.h"
#include <Arduino.h>

MotorControl::MotorControl(int detectpin,int outpin1,int outpin2)
: _detectpin(detectpin), _outpin1(outpin1), _outpin2(outpin2){

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

void MotorControl::setSpeed(int speed) {
    if (speed > 0) {
        digitalWrite(_detectpin,HIGH);
        analogWrite(_outpin2, speed);
        digitalWrite(_outpin1, LOW);
    } else if (speed < 0) {
        digitalWrite(_detectpin,HIGH);
        digitalWrite(_outpin2, LOW);
        analogWrite(_outpin1, -speed);
    } else {
        digitalWrite(_detectpin,HIGH);
        digitalWrite(_outpin1, LOW);
        digitalWrite(_outpin2, LOW);
    }
}