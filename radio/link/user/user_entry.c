//
// Created by ma on 2020/11/12.
//



#include "../../port_interfaces/x86_64_linux/port_interfaces.h"
#include "../../common/config.h"
#include "../../phy/radio_rx.h"

#include "../../phy/filters/fdacoefs_FIR_1000.h"
#include "../../phy/filters/fdacoefs_FIR_1500.h"
#include "../../phy/filters/fdacoefs_FIR_2000.h"
#include "../../phy/filters/fdacoefs_FIR_2500.h"
#include "../../phy/radio_tx.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

const static SIGNAL kernels[4]={&FIR_KERNEL_1000,&FIR_KERNEL_1500,&FIR_KERNEL_2000,&FIR_KERNEL_2500};

int8_t channel=-1;
static BIT_STREAM sync_header=NULL;

static void time_limiter(void (*function(void)),double ms,char* name){
    double begin=interface_get_current_ms();
    function();
    double time_left=ms-(interface_get_current_ms()-begin);
    if(time_left<0.0){
        printf("Warning: timeout in when executing %s\n",name);
    }else{
        interface_delay_ms(time_left);
    }
}


double user_get_sync_header_bias(){
    double begin=interface_get_current_ms();

    SIGNAL signal= interface_get_pcm_sync(TIME_UP_MS);
    signal_normalize_to(signal);
    signal_broadcast_add_to(-0.5,signal);
    signal_broadcast_multiply_to(2.0,signal);
    signal->boundary_type=SIGNAL_BOUNDARY_TYPE_ZERO_PADDING;
    SIGNAL ch=signal_convolve(signal,kernels[channel],SIGNAL_CONVOLVE_MODE_SAME);
    signal_del(signal);
    ch->boundary_type=SIGNAL_BOUNDARY_TYPE_ZERO_PADDING;
    SIGNAL dc=signal_moving_average(ch,20,SIGNAL_CONVOLVE_MODE_SAME);
    signal_del(ch);
    BIT_STREAM bin=radio_rx_binarize(dc,0.5);
    signal_del(dc);
    int pos=radio_rx_search_sync_header(bin,sync_header,0);
    bit_stream_del(bin);
    double end=interface_get_current_ms();

    double calc_cost_time_ms=end-begin;

    if(calc_cost_time_ms>TIME_DOWN_D_MS){
        printf("Warning: Current decodeing process costs more time than excepted (%d ms). REQUIRED processing time %dms",(int)calc_cost_time_ms,(int)TIME_DOWN_D_MS);
    }

    double sync_bias_time_ms=1000.0*pos/FS;
    return TIME_DOWN_MS+TIME_DOWN_D_MS+sync_bias_time_ms-calc_cost_time_ms;
}
#define SYNC_TOLERANCE 20.0
void user_init(int channel_num){
    sync_header=radio_tx_generate_sync_header();
    interface_user_init();
    channel=channel_num;
    printf("Syncing...\n");
    int ctr=0;
    while(ctr<4)
    {
        double bias=0.0;
        int try=0;
        do{


            try++;
            bias=user_get_sync_header_bias();
            float foo=(float) bias/TIME_UP_MS;
            float time_to_delay=(float) bias;
            if(foo>1.0){
                time_to_delay-=TIME_UP_MS*(int)foo;
            }
            interface_delay_ms(time_to_delay);
            printf("delay:%d\t",(int)time_to_delay);
            printf("bias:%d\n",(int)(bias-TIME_UP_MS));

        }while(fabs(bias-TIME_UP_MS)>SYNC_TOLERANCE);
//        printf("ctr:%d\n",ctr);
        if(try>1) ctr=0; else ctr++;
    }
    printf("Sync complete successfully.\n");

}

void user_up(){
PACKAGE package=interface_user_get_package();
SIGNAL signal=package_to_signal(channel,package);
int16_t *buffer=malloc(sizeof(int16_t)*signal->length);
signal_write_pcm16_to_buffer(buffer,signal,0);

interface_play_pcm_sync(buffer,signal->length);
signal_del(signal);
free(buffer);

}
void user_down(){
    interface_delay_ms(TIME_UP_MS);
}

void user_main(int argc,char* argv[]){

    if(argc==2){
        user_init(strtoul(argv[1],0,0));
        while (1){
            interface_delay_ms(TIME_DOWN_D_MS);
            time_limiter(user_up,TIME_DOWN_MS,"USER_UP");
            interface_delay_ms(TIME_DOWN_D_MS);
            time_limiter(user_down,TIME_UP_MS,"USER_DOWM");


        }

    }
}