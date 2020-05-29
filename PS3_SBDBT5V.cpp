#include "mbed.h"
#include "PS3_SBDBT5V.h"

PS3::PS3(PinName tx, PinName rx)
    : _serial_p(new Serial(tx, rx)), _serial(*_serial_p)
{
    _serial.baud(2400);
    FREE[0] = 0x80;
    FREE[1] = 0x00;
    FREE[2] = 0x00;
    FREE[3] = 0x40;
    FREE[4] = 0x40;
    FREE[5] = 0x40;
    FREE[6] = 0x40;
    FREE[7] = 0x00;
    check2 = 0;
    j = 0;
}

PS3::PS3(Serial &serial_obj) : _serial_p(NULL), _serial(serial_obj)
{
    _serial.baud(2400);
    FREE[0] = 0x80;
    FREE[1] = 0x00;
    FREE[2] = 0x00;
    FREE[3] = 0x40;
    FREE[4] = 0x40;
    FREE[5] = 0x40;
    FREE[6] = 0x40;
    FREE[7] = 0x00;
    check2 = 0;
    j = 0;
}

PS3::~PS3()
{
    delete _serial_p;
}

int PS3::get_data()
{
    check = 0;
    if(_serial.readable()) {
        check = 1;
        check2 = 1;
        for(i=0; i<8; i++) {
            data[i] = _serial.getc();
            if(data[i] == FREE[i]){
                j++;
            }
        }
        if(j == 8){
            return -1;
        }else{
            reference();
        }
        
    }
    return check;
}

void PS3::reference()
{
    result[UP]       = (data[2] & 0x01)?1:0;
    result[DOWN]     = (data[2] & 0x02)?1:0;
    result[RIGHT]    = (data[2] & 0x04)?1:0;
    result[LEFT]     = (data[2] & 0x08)?1:0;
    result[TRIANGLE] = (data[2] & 0x10)?1:0;
    result[CROSS]    = (data[2] & 0x20)?1:0;
    result[CIRCLE]   = (data[2] & 0x40)?1:0;
    result[SQUARE]   = (data[1] & 0x01)?1:0;
    result[L1]       = (data[1] & 0x01)?1:0;
    result[L2]       = (data[1] & 0x01)?1:0;
    result[R1]       = (data[1] & 0x01)?1:0;
    result[R2]       = (data[1] & 0x01)?1:0;
    
    if(data[3] == 0x40){                                // 左アナログスティック左右
        result[LEFT_ANALOG_Y] = NEUTRAL;
    }else if( (data[3] < 0x40)&&(data[3] >= 0x20) ){
        result[LEFT_ANALOG_Y] = LOW;
    }else if( (data[3] < 0x20)&&(data[3] >= 0x10) ){
        result[LEFT_ANALOG_Y] = MIDDLE;
    }else if( (data[3] < 0x10)&&(data[3] >= 0x00) ){
        result[LEFT_ANALOG_Y] = HIGH;
    }else if( (data[3] <= 0x60)&&(data[3] > 0x40) ){
        result[LEFT_ANALOG_Y] = LOW;
    }else if( (data[3] <= 0x70)&&(data[3] > 0x60) ){
        result[LEFT_ANALOG_Y] = MIDDLE;
    }else if( (data[3] < 0x80)&&(data[3] > 0x70) ){
        result[LEFT_ANALOG_Y] = HIGH;
    }
    
    if(data[4] == 0x40){                                // 左アナログスティック上下
        result[LEFT_ANALOG_X] = NEUTRAL;
    }else if( (data[4] < 0x40)&&(data[3] >= 0x20) ){
        result[LEFT_ANALOG_X] = LOW;
    }else if( (data[4] < 0x20)&&(data[3] >= 0x10) ){
        result[LEFT_ANALOG_X] = MIDDLE;
    }else if( (data[4] < 0x10)&&(data[3] >= 0x00) ){
        result[LEFT_ANALOG_X] = HIGH;
    }else if( (data[4] <= 0x60)&&(data[3] > 0x40) ){
        result[LEFT_ANALOG_X] = LOW;
    }else if( (data[4] <= 0x70)&&(data[3] > 0x60) ){
        result[LEFT_ANALOG_X] = MIDDLE;
    }else if( (data[4] < 0x80)&&(data[3] > 0x70) ){
        result[LEFT_ANALOG_X] = HIGH;
    }
    
    if(data[5] == 0x40){                                // 右アナログスティック左右
        result[RIGHT_ANALOG_Y] = NEUTRAL;
    }else if( (data[5] < 0x40)&&(data[3] >= 0x20) ){
        result[RIGHT_ANALOG_Y] = LOW;
    }else if( (data[5] < 0x20)&&(data[3] >= 0x10) ){
        result[RIGHT_ANALOG_Y] = MIDDLE;
    }else if( (data[5] < 0x10)&&(data[3] >= 0x00) ){
        result[RIGHT_ANALOG_Y] = HIGH;
    }else if( (data[5] <= 0x60)&&(data[3] > 0x40) ){
        result[RIGHT_ANALOG_Y] = LOW;
    }else if( (data[5] <= 0x70)&&(data[3] > 0x60) ){
        result[RIGHT_ANALOG_Y] = MIDDLE;
    }else if( (data[5] < 0x80)&&(data[3] > 0x70) ){
        result[RIGHT_ANALOG_Y] = HIGH;
    }
    
    if(data[6] == 0x40){                                // 右アナログスティック上下
        result[RIGHT_ANALOG_X] = NEUTRAL;
    }else if( (data[6] < 0x40)&&(data[3] >= 0x20) ){
        result[RIGHT_ANALOG_X] = LOW;
    }else if( (data[6] < 0x20)&&(data[3] >= 0x10) ){
        result[RIGHT_ANALOG_X] = MIDDLE;
    }else if( (data[6] < 0x10)&&(data[3] >= 0x00) ){
        result[RIGHT_ANALOG_X] = HIGH;
    }else if( (data[6] <= 0x60)&&(data[3] > 0x40) ){
        result[RIGHT_ANALOG_X] = LOW;
    }else if( (data[6] <= 0x70)&&(data[3] > 0x60) ){
        result[RIGHT_ANALOG_X] = MIDDLE;
    }else if( (data[6] < 0x80)&&(data[3] > 0x70) ){
        result[RIGHT_ANALOG_X] = HIGH;
    }
}

int PS3::get_analog(int analog){
    if(check2 == 0){
        return 0x40;
    }else{
        return data[analog-11];
    }
}