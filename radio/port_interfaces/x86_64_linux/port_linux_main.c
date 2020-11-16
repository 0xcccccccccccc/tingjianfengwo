//
// Created by ma on 2020/11/13.
//

#include "../../link/station/station_entry.h"
#include "../../link/user/user_entry.h"
#include "port_interfaces.h"
#include <stdio.h>
#include <time.h>
#include "../../phy/radio_tx.h"
#include "../../phy/debug.h"
char *arg[2]={"user_main","2"};
static uint8_t pac[]={0b01110010,'h','i','1'};
static uint8_t pac2[]={0b01110010,'h','i','2'};
static uint8_t pac3[]={0b01110010,'h','i','3'};
static uint8_t pac4[]={0b01110010,'h','i','4'};
int main(){
    //station_main(1,NULL);
    //user_main(2,arg);
//    struct timespec time_start={0, 0},time_end={0, 0};
//    clock_gettime(CLOCK_REALTIME, &time_start);
//    printf("start time %llus,%llu ns\n", time_start.tv_sec, time_start.tv_nsec);
//
//    for(int i=0;i<100;i++)
//        interface_delay_ms(100);
//
//    clock_gettime(CLOCK_REALTIME, &time_end);
//    printf("endtime %llus,%llu ns\n", time_end.tv_sec, time_end.tv_nsec);
//    printf("duration:%llus %lluns\n", time_end.tv_sec-time_start.tv_sec, time_end.tv_nsec-time_start.tv_nsec);
SIGNAL signal=radio_tx_generate_signal_from_bytes(800,&pac,4);
    SIGNAL signal2=radio_tx_generate_signal_from_bytes(1000,&pac2,4);
    SIGNAL signal3=radio_tx_generate_signal_from_bytes(1333,&pac3,4);
    SIGNAL signal4=radio_tx_generate_signal_from_bytes(2000,&pac4,4);
    signal_add_to(signal2,signal);
    signal_add_to(signal3,signal);
    signal_add_to(signal4,signal);

    debug_signal_save_pcm_wav("out.wav",signal);

}