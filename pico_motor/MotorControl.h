#ifndef MOTORCONTROL_H
#define MOTORCONTROL_H

class MotorControl{
    public:
        MotorControl(int detectpin,int outpin1,int outpin2);
        void forward();
        void reverse();
        void stop();
        void setSpeed(int speed);
        void reset();
    private:
        int _detectpin;
        int _outpin1;
        int _outpin2;
};

#endif