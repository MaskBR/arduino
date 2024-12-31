#ifndef MOTORCONTROL_H
#define MOTORCONTROL_H
#include <WString.h>

class MotorControl{
    public:
        MotorControl(int detectpin,int outpin1,int outpin2, String motorname = "DefaultMotor");
        void forward();
        void reverse();
        void stop();
        void reset();
        String getMotorName() const { return _motorname; }
    private:
        int _detectpin;
        int _outpin1;
        int _outpin2;
        String _motorname;
};

#endif