// version 2.4 //
#include "mbed.h"
#include "PS3_SBDBT5V.h"

PS3::PS3(PinName rx, PinName connect, int rate)
    : Serial(NC, rx, rate), _connect(connect)
{
    initialization();
}

void PS3::initialization()
{
    FREE[0] = 0x80;
    FREE[1] = 0x00;
    FREE[2] = 0x00;
    FREE[3] = 0x40;
    FREE[4] = 0x40;
    FREE[5] = 0x40;
    FREE[6] = 0x40;
    FREE[7] = 0x00;
    for(int i=0; i<8; i++) {
        ps3_data[i] = FREE[i];
    }

}

int PS3::get_data(int* data_p)
{
    j = 0;
    if(readable()) {
        for(int i=0; i<8; i++) {
            /*#if MBED_MAJOR_VERSION >= 6
                        read(&ps3_data[i], 1);
            //#else*/
            ps3_data[i] = getc();
//#endif
            if(ps3_data[i] == FREE[i]) {
                j++;
            }
        }
        if(j == 8) {
            for(int i=0; i<MAX_BUTTON; i++) {
                data_p[i] = 0;
            }
            return -1;
        } else {
            reference();
            for(int i=0; i<MAX_BUTTON; i++) {
                data_p[i] = result[i];
            }
            return 1;
        }
    } else {
        return 0;
    }
}

void PS3::reference()
{
    if(ps3_data[2] == 0x03){            // start
        result[START]    = 1;
        result[SELECT]   = 0;
        result[UP]       = 0;
        result[DOWN]     = 0;
        result[RIGHT]    = 0;
        result[LEFT]     = 0;
    } else if(ps3_data[2] == 0x07){     // start + right
        result[START]    = 1;
        result[SELECT]   = 0;
        result[UP]       = 0;
        result[DOWN]     = 0;
        result[RIGHT]    = 1;
        result[LEFT]     = 0;
    } else if(ps3_data[2] == 0x0b){     // start + left
        result[START]    = 1;
        result[SELECT]   = 0;
        result[UP]       = 0;
        result[DOWN]     = 0;
        result[RIGHT]    = 0;
        result[LEFT]     = 1;
    } else if(ps3_data[2] == 0x0c){     // select
        result[START]    = 0;
        result[SELECT]   = 1;
        result[UP]       = 0;
        result[DOWN]     = 0;
        result[RIGHT]    = 0;
        result[LEFT]     = 0;
    } else if(ps3_data[2] == 0x0d){     // select + up
        result[START]    = 0;
        result[SELECT]   = 1;
        result[UP]       = 1;
        result[DOWN]     = 0;
        result[RIGHT]    = 0;
        result[LEFT]     = 0;
    } else if(ps3_data[2] == 0x0e){     // select + down
        result[START]    = 0;
        result[SELECT]   = 1;
        result[UP]       = 0;
        result[DOWN]     = 1;
        result[RIGHT]    = 0;
        result[LEFT]     = 0;
    } else {                            // not pushing start & select
        result[START]    = 0;
        result[SELECT]   = 0;
        result[UP]       = (ps3_data[2] & 0x01)?1:0;
        result[DOWN]     = (ps3_data[2] & 0x02)?1:0;
        result[RIGHT]    = (ps3_data[2] & 0x04)?1:0;
        result[LEFT]     = (ps3_data[2] & 0x08)?1:0;
    }
        
    result[TRIANGLE] = (ps3_data[2] & 0x10)?1:0;
    result[CROSS]    = (ps3_data[2] & 0x20)?1:0;
    result[CIRCLE]   = (ps3_data[2] & 0x40)?1:0;
    result[SQUARE]   = (ps3_data[1] & 0x01)?1:0;
    result[L1]       = (ps3_data[1] & 0x02)?1:0;
    result[L2]       = (ps3_data[1] & 0x04)?1:0;
    result[R1]       = (ps3_data[1] & 0x08)?1:0;
    result[R2]       = (ps3_data[1] & 0x10)?1:0;

    if(ps3_data[3] == 0x40) {                               // 左アナログスティックX軸
        result[LEFT_ANALOG_X] = NEUTRAL;
    } else if( (ps3_data[3] < 0x40)&&(ps3_data[3] >= 0x20) ) {
        result[LEFT_ANALOG_X] = L_LOW;
    } else if( (ps3_data[3] < 0x20)&&(ps3_data[3] >= 0x10) ) {
        result[LEFT_ANALOG_X] = L_MIDDLE;
    } else if( (ps3_data[3] < 0x10)&&(ps3_data[3] >= 0x00) ) {
        result[LEFT_ANALOG_X] = L_HIGH;
    } else if( (ps3_data[3] <= 0x60)&&(ps3_data[3] > 0x40) ) {
        result[LEFT_ANALOG_X] = R_LOW;
    } else if( (ps3_data[3] <= 0x70)&&(ps3_data[3] > 0x60) ) {
        result[LEFT_ANALOG_X] = R_MIDDLE;
    } else if( (ps3_data[3] < 0x80)&&(ps3_data[3] > 0x70) ) {
        result[LEFT_ANALOG_X] = R_HIGH;
    }

    if(ps3_data[4] == 0x40) {                               // 左アナログスティックY軸
        result[LEFT_ANALOG_Y] = NEUTRAL;
    } else if( (ps3_data[4] < 0x40)&&(ps3_data[4] >= 0x20) ) {
        result[LEFT_ANALOG_Y] = U_LOW;
    } else if( (ps3_data[4] < 0x20)&&(ps3_data[4] >= 0x10) ) {
        result[LEFT_ANALOG_Y] = U_MIDDLE;
    } else if( (ps3_data[4] < 0x10)&&(ps3_data[4] >= 0x00) ) {
        result[LEFT_ANALOG_Y] = U_HIGH;
    } else if( (ps3_data[4] <= 0x60)&&(ps3_data[4] > 0x40) ) {
        result[LEFT_ANALOG_Y] = D_LOW;
    } else if( (ps3_data[4] <= 0x70)&&(ps3_data[4] > 0x60) ) {
        result[LEFT_ANALOG_Y] = D_MIDDLE;
    } else if( (ps3_data[4] < 0x80)&&(ps3_data[4] > 0x70) ) {
        result[LEFT_ANALOG_Y] = D_HIGH;
    }

    if(ps3_data[5] == 0x40) {                               // 右アナログスティックX軸
        result[RIGHT_ANALOG_X] = NEUTRAL;
    } else if( (ps3_data[5] < 0x40)&&(ps3_data[5] >= 0x20) ) {
        result[RIGHT_ANALOG_X] = L_LOW;
    } else if( (ps3_data[5] < 0x20)&&(ps3_data[5] >= 0x10) ) {
        result[RIGHT_ANALOG_X] = L_MIDDLE;
    } else if( (ps3_data[5] < 0x10)&&(ps3_data[5] >= 0x00) ) {
        result[RIGHT_ANALOG_X] = L_HIGH;
    } else if( (ps3_data[5] <= 0x60)&&(ps3_data[5] > 0x40) ) {
        result[RIGHT_ANALOG_X] = R_LOW;
    } else if( (ps3_data[5] <= 0x70)&&(ps3_data[5] > 0x60) ) {
        result[RIGHT_ANALOG_X] = R_MIDDLE;
    } else if( (ps3_data[5] < 0x80)&&(ps3_data[5] > 0x70) ) {
        result[RIGHT_ANALOG_X] = R_HIGH;
    }

    if(ps3_data[6] == 0x40) {                               // 右アナログスティックY軸
        result[RIGHT_ANALOG_Y] = NEUTRAL;
    } else if( (ps3_data[6] < 0x40)&&(ps3_data[6] >= 0x20) ) {
        result[RIGHT_ANALOG_Y] = U_LOW;
    } else if( (ps3_data[6] < 0x20)&&(ps3_data[6] >= 0x10) ) {
        result[RIGHT_ANALOG_Y] = U_MIDDLE;
    } else if( (ps3_data[6] < 0x10)&&(ps3_data[6] >= 0x00) ) {
        result[RIGHT_ANALOG_Y] = U_HIGH;
    } else if( (ps3_data[6] <= 0x60)&&(ps3_data[6] > 0x40) ) {
        result[RIGHT_ANALOG_Y] = D_LOW;
    } else if( (ps3_data[6] <= 0x70)&&(ps3_data[6] > 0x60) ) {
        result[RIGHT_ANALOG_Y] = D_MIDDLE;
    } else if( (ps3_data[6] < 0x80)&&(ps3_data[6] > 0x70) ) {
        result[RIGHT_ANALOG_Y] = D_HIGH;
    }
}

void PS3::get_analog(int* analog)
{
    for(int i=0; i<4; i++) {
        analog[i] = ps3_data[i + 3];
    }
}

int PS3::check_connection()
{
    return _connect;
}