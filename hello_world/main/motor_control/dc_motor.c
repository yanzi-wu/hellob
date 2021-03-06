#include "dc_motor.h"
#include "stdio.h"
#include "mpu_6050.h"

extern void set_car_move(uint8_t straight,uint8_t turn);

void get_car_move_direction(uint8_t *p_data,uint8_t len)
{
    CAR_TO_MOVE *p_car_move;
    if(len == sizeof(CAR_TO_MOVE)){
        p_car_move = (CAR_TO_MOVE *)p_data;
        set_car_move(p_car_move->to_forward,p_car_move->to_left);
        printf("car forward:%d,   left:%d\r\n",p_car_move->to_forward,p_car_move->to_left);
    }
    else{
        printf("[%s] receive data is not correct in length\r\n",__func__);
    }
}
