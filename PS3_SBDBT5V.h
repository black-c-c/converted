#ifndef _PS3_SBDBT5V_H_
#define _PS3_SBDBT5V_H_

#include "mbed.h"
#include "PS3_SBDBT5V.h"

class PS3
{
public:
    
    enum{
        UP,
        DOWN,
        RIGHT,
        LEFT,
        TRIANGLE,
        CROSS,
        CIRCLE,
        SQUARE,
        L1,
        L2,
        R1,
        R2,
        START,
        SELECT,
        LEFT_ANALOG_Y,//14
        LEFT_ANALOG_X,
        RIGHT_ANALOG_Y,
        RIGHT_ANALOG_X,
        MAX_BUTTON
    };
    
    enum{
        HIGH = -3,
        MIDDLE,
        LOW,
        NEUTRAL
    };
    
    PS3(PinName tx, PinName rx);
    
    PS3(Serial &serial_obj);
    
    ~PS3();
    
    int result[MAX_BUTTON];
    
    //int get_result();
    int get_data();
    int get_analog(int analog);
    
private:
    
    Serial *_serial_p;
    Serial &_serial;
    
    int i;
    int j;
    int check;
    int check2;
    int data[8];
    int FREE[8];
    void reference();
    
};

#endif