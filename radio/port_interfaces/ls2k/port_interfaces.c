//
// Created by ma on 2020/11/13.
//
#include <rtthread.h>
#include "port_interfaces.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>

#include "../../phy/debug.h"
#include "../../../vs1003/vs1003.h"
#include <drivers/pin.h>
static int amplifer_enable()
{
  rt_pin_mode(21,0);
  rt_pin_write(21,0);

}
static int amplifer_disable()
{
   rt_pin_mode(21,0);
   rt_pin_write(21,1);
}

void interface_delay_ms(float ms){

    rt_thread_mdelay((int)ms);

}


void interface_play_pcm_sync(int16_t *data, int32_t length){
    amplifer_enable();
    vs1003_play_chunk(data,length);
    amplifer_disable();
}
SIGNAL interface_get_pcm_sync(double ms){

}
double interface_get_current_ms(){
    return 1000.0*rt_tick_get()/RT_TICK_PER_SECOND;
}

void interface_user_init(){

}
void interface_station_init(){
vs1003_play_mode_init();

}
PACKAGE interface_user_get_package(){
    return package_new(0xb0,'i');
}