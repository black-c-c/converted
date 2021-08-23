#include "mbed.h"
#include "PS3_SBDBT5V.h"

PS3::PS3(PinName rx, int baud)
    : Serial(NC, rx, baud)
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
    for(i=0; i<8; i++) {
        ps3_data[i] = FREE[i];
    }

}

int PS3::get_data(int* data_p)
{
    j = 0;
    if(readable()) {
        for(i=0; i<8; i++) {
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
            for(i=0; i<MAX_BUTTON; i++) {
                data_p[i] = 0;
            }
            return -1;
        } else {
            reference();
            for(i=0; i<MAX_BUTTON; i++) {
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
    result[UP]       = (ps3_data[2] & 0x01)?1:0;
    result[DOWN]     = (ps3_data[2] & 0x02)?1:0;
    result[RIGHT]    = (ps3_data[2] & 0x04)?1:0;
    result[LEFT]     = (ps3_data[2] & 0x08)?1:0;
    result[TRIANGLE] = (ps3_data[2] & 0x10)?1:0;
    result[CROSS]    = (ps3_data[2] & 0x20)?1:0;
    result[CIRCLE]   = (ps3_data[2] & 0x40)?1:0;
    result[SQUARE]   = (ps3_data[1] & 0x01)?1:0;
    result[L1]       = (ps3_data[1] & 0x02)?1:0;
    result[L2]       = (ps3_data[1] & 0x04)?1:0;
    result[R1]       = (ps3_data[1] & 0x08)?1:0;
    result[R2]       = (ps3_data[1] & 0x10)?1:0;
    result[START]    = (ps3_data[2] & 0x03)?1:0;
    result[SELECT]   = (ps3_data[2] & 0x0c)?1:0;

    if(ps3_data[3] == 0x40) {                               // 左アナログスティック上下
        result[LEFT_ANALOG_Y] = NEUTRAL;
    } else if( (ps3_data[3] < 0x40)&&(ps3_data[3] >= 0x20) ) {
        result[LEFT_ANALOG_Y] = U_LOW;
    } else if( (ps3_data[3] < 0x20)&&(ps3_data[3] >= 0x10) ) {
        result[LEFT_ANALOG_Y] = U_MIDDLE;
    } else if( (ps3_data[3] < 0x10)&&(ps3_data[3] >= 0x00) ) {
        result[LEFT_ANALOG_Y] = U_HIGH;
    } else if( (ps3_data[3] <= 0x60)&&(ps3_data[3] > 0x40) ) {
        result[LEFT_ANALOG_Y] = D_LOW;
    } else if( (ps3_data[3] <= 0x70)&&(ps3_data[3] > 0x60) ) {
        result[LEFT_ANALOG_Y] = D_MIDDLE;
    } else if( (ps3_data[3] < 0x80)&&(ps3_data[3] > 0x70) ) {
        result[LEFT_ANALOG_Y] = D_HIGH;
    }

    if(ps3_data[4] == 0x40) {                               // 左アナログスティック左右
        result[LEFT_ANALOG_X] = NEUTRAL;
    } else if( (ps3_data[4] < 0x40)&&(ps3_data[4] >= 0x20) ) {
        result[LEFT_ANALOG_X] = L_LOW;
    } else if( (ps3_data[4] < 0x20)&&(ps3_data[4] >= 0x10) ) {
        result[LEFT_ANALOG_X] = L_MIDDLE;
    } else if( (ps3_data[4] < 0x10)&&(ps3_data[4] >= 0x00) ) {
        result[LEFT_ANALOG_X] = L_HIGH;
    } else if( (ps3_data[4] <= 0x60)&&(ps3_data[4] > 0x40) ) {
        result[LEFT_ANALOG_X] = R_LOW;
    } else if( (ps3_data[4] <= 0x70)&&(ps3_data[4] > 0x60) ) {
        result[LEFT_ANALOG_X] = R_MIDDLE;
    } else if( (ps3_data[4] < 0x80)&&(ps3_data[4] > 0x70) ) {
        result[LEFT_ANALOG_X] = R_HIGH;
    }

    if(ps3_data[5] == 0x40) {                               // 右アナログスティック上下
        result[RIGHT_ANALOG_Y] = NEUTRAL;
    } else if( (ps3_data[5] < 0x40)&&(ps3_data[5] >= 0x20) ) {
        result[RIGHT_ANALOG_Y] = U_LOW;
    } else if( (ps3_data[5] < 0x20)&&(ps3_data[5] >= 0x10) ) {
        result[RIGHT_ANALOG_Y] = U_MIDDLE;
    } else if( (ps3_data[5] < 0x10)&&(ps3_data[5] >= 0x00) ) {
        result[RIGHT_ANALOG_Y] = U_HIGH;
    } else if( (ps3_data[5] <= 0x60)&&(ps3_data[5] > 0x40) ) {
        result[RIGHT_ANALOG_Y] = D_LOW;
    } else if( (ps3_data[5] <= 0x70)&&(ps3_data[5] > 0x60) ) {
        result[RIGHT_ANALOG_Y] = D_MIDDLE;
    } else if( (ps3_data[5] < 0x80)&&(ps3_data[5] > 0x70) ) {
        result[RIGHT_ANALOG_Y] = D_HIGH;
    }

    if(ps3_data[6] == 0x40) {                               // 右アナログスティック左右
        result[RIGHT_ANALOG_X] = NEUTRAL;
    } else if( (ps3_data[6] < 0x40)&&(ps3_data[6] >= 0x20) ) {
        result[RIGHT_ANALOG_X] = L_LOW;
    } else if( (ps3_data[6] < 0x20)&&(ps3_data[6] >= 0x10) ) {
        result[RIGHT_ANALOG_X] = L_MIDDLE;
    } else if( (ps3_data[6] < 0x10)&&(ps3_data[6] >= 0x00) ) {
        result[RIGHT_ANALOG_X] = L_HIGH;
    } else if( (ps3_data[6] <= 0x60)&&(ps3_data[6] > 0x40) ) {
        result[RIGHT_ANALOG_X] = R_LOW;
    } else if( (ps3_data[6] <= 0x70)&&(ps3_data[6] > 0x60) ) {
        result[RIGHT_ANALOG_X] = R_MIDDLE;
    } else if( (ps3_data[6] < 0x80)&&(ps3_data[6] > 0x70) ) {
        result[RIGHT_ANALOG_X] = R_HIGH;
    }
}

int PS3::get_analog(int analog)
{
    if( (analog > SELECT) && (analog < MAX_BUTTON) ) {
        return ps3_data[analog-11];
    } else {
        return -1;
    }
}