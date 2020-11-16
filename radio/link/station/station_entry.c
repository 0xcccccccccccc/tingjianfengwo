//
// Created by ma on 2020/11/12.
//

#include "../../common/package_t/package_t.h"
#include "../../common/queue/queue.h"
#include "../../common/config.h"
#include "../../phy/radio_tx.h"
#include "../../phy/radio_rx.h"
#include "../../common/wav_helper/wav_helper.h"


#include "../../phy/filters/fdacoefs_FIR_1000.h"
#include "../../phy/filters/fdacoefs_FIR_1500.h"
#include "../../phy/filters/fdacoefs_FIR_2000.h"
#include "../../phy/filters/fdacoefs_FIR_2500.h"

#include "../../phy/debug.h"
#include "../../port_interfaces/x86_64_linux/port_interfaces.h"


DEFINE_Q_GET(queue_get_package, PACKAGE)
DEFINE_Q_DESTROY(queue_destroy_complete_package, PACKAGE)
DEFINE_Q_PUT(queue_put_package, PACKAGE)
queue_t* channels_queue[4];

#define PACKAGE_TX_LENGTH (TONE_LENGTH+TONE_SPACING_LENGTH)*8*4

const static int channel_freqs[4]={CH1_FREQ,CH2_FREQ,CH3_FREQ,CH4_FREQ};
static BIT_STREAM sync_header=NULL;
void station_init(){
    interface_station_init();
    sync_header=radio_tx_generate_sync_header();
    for(int i=0;i<4;i++){
        channels_queue[i]=queue_create();
    }
}

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

void station_down(){
    SIGNAL signal=signal_new(PACKAGE_TX_LENGTH, 0.0, SIGNAL_BOUNDARY_TYPE_FIXED);
    for(int i=0;i<4;i++){
        if(queue_elements(channels_queue[i])>0){
            PACKAGE *current_package=NULL;
            queue_get_package(channels_queue[i],&current_package);
            SIGNAL current_signal=package_to_signal(channel_freqs[i],*current_package);
            free(current_package);
            signal_add_to(current_signal,signal);
            signal_del(current_signal);
        }
        else{
            SIGNAL current_signal=package_to_signal(i,NULL_PACKAGE);
            signal_add_to(current_signal,signal);
            signal_del(current_signal);
        }
    }

    signal_normalize_to(signal);
    signal_broadcast_add_to(-0.5,signal);
    signal_broadcast_multiply_to(2.0*32767.0,signal);
//    int16_t *buffer=malloc(PACKAGE_TX_LENGTH * sizeof(int16_t) + 22);

//    signal_write_pcm16_to_buffer(buffer,signal,22);// redunctant argument: bias
//    write_wav_header_to_buffer(buffer, 1, 16, 8000, PACKAGE_TX_LENGTH);
    int16_t *buffer=malloc(PACKAGE_TX_LENGTH * sizeof(int16_t));
    signal_write_pcm16_to_buffer(buffer,signal,0);
    interface_play_pcm_sync(buffer, PACKAGE_TX_LENGTH);
    free(buffer);
    signal_del(signal);
}
void station_up(){
    SIGNAL signal= interface_get_pcm_sync(TIME_UP_MS);
    signal->boundary_type=SIGNAL_BOUNDARY_TYPE_ZERO_PADDING;
    SIGNAL ch1=signal_convolve(signal,&FIR_KERNEL_1000,SIGNAL_CONVOLVE_MODE_SAME);
    SIGNAL ch2=signal_convolve(signal,&FIR_KERNEL_1500,SIGNAL_CONVOLVE_MODE_SAME);
    SIGNAL ch3=signal_convolve(signal,&FIR_KERNEL_2000,SIGNAL_CONVOLVE_MODE_SAME);
    SIGNAL ch4=signal_convolve(signal,&FIR_KERNEL_2500,SIGNAL_CONVOLVE_MODE_SAME);

    signal_del(signal);

    ch1->boundary_type=SIGNAL_BOUNDARY_TYPE_ZERO_PADDING;
    ch2->boundary_type=SIGNAL_BOUNDARY_TYPE_ZERO_PADDING;
    ch3->boundary_type=SIGNAL_BOUNDARY_TYPE_ZERO_PADDING;
    ch4->boundary_type=SIGNAL_BOUNDARY_TYPE_ZERO_PADDING;


    SIGNAL ma1=signal_moving_average(ch1,20,SIGNAL_CONVOLVE_MODE_SAME);
    SIGNAL ma2=signal_moving_average(ch2,20,SIGNAL_CONVOLVE_MODE_SAME);
    SIGNAL ma3=signal_moving_average(ch3,20,SIGNAL_CONVOLVE_MODE_SAME);
    SIGNAL ma4=signal_moving_average(ch4,20,SIGNAL_CONVOLVE_MODE_SAME);

    signal_del(ch1);signal_del(ch2);signal_del(ch3);signal_del(ch4);


    signal_normalize_to(ma1);
    signal_normalize_to(ma2);
    signal_normalize_to(ma3);
    signal_normalize_to(ma4);


    BIT_STREAM  b1=radio_rx_binarize(ma1,0.45);
    BIT_STREAM  b2=radio_rx_binarize(ma2,0.45);
    BIT_STREAM  b3=radio_rx_binarize(ma3,0.45);
    BIT_STREAM  b4=radio_rx_binarize(ma4,0.45);
    b1->boundary_type=BIT_STREAM_BOUNDARY_ZERO_PADDING;
    b2->boundary_type=BIT_STREAM_BOUNDARY_ZERO_PADDING;
    b3->boundary_type=BIT_STREAM_BOUNDARY_ZERO_PADDING;
    b4->boundary_type=BIT_STREAM_BOUNDARY_ZERO_PADDING;

    signal_del(ma1);
    signal_del(ma2);
    signal_del(ma3);
    signal_del(ma4);


    int h1=radio_rx_search_sync_header(b1,sync_header,0);
    int h2=radio_rx_search_sync_header(b2,sync_header,0);
    int h3=radio_rx_search_sync_header(b3,sync_header,0);
    int h4=radio_rx_search_sync_header(b4,sync_header,0);
    printf("h1:%d\th2:%d\th3:%d\th4:%d\n",h1,h2,h3,h4);


    PACKAGE *p1=package_alloc_decode_from_bit_stream(b1,h1);
    PACKAGE *p2=package_alloc_decode_from_bit_stream(b2,h2);
    PACKAGE *p3=package_alloc_decode_from_bit_stream(b3,h3);
    PACKAGE *p4=package_alloc_decode_from_bit_stream(b4,h4);

    if(p1!=NULL){
        uint8_t src=package_network_get_src(*p1);
        uint8_t dst=package_network_get_dst(*p1);
        if(src!=dst && src==0)
        queue_put_package(channels_queue[0],p1);
    }
    if(p2!=NULL){
        uint8_t src=package_network_get_src(*p2);
        uint8_t dst=package_network_get_dst(*p2);
        if(src!=dst && src==1)
            queue_put_package(channels_queue[1],p2);
    }
    if(p3!=NULL){
        uint8_t src=package_network_get_src(*p3);
        uint8_t dst=package_network_get_dst(*p3);
        if(src!=dst && src==2)
            queue_put_package(channels_queue[2],p3);
    }
    if(p4!=NULL){
        uint8_t src=package_network_get_src(*p4);
        uint8_t dst=package_network_get_dst(*p4);
        if(src!=dst && src==3)
            queue_put_package(channels_queue[3],p4);
    }

    bit_stream_del(b1);
    bit_stream_del(b2);
    bit_stream_del(b3);
    bit_stream_del(b4);

}
int station_main(int argc,char* argv[]){
    station_init();
    while(1){
        interface_delay_ms(TIME_DOWN_D_MS);
        time_limiter(station_down,TIME_DOWN_MS,"STATION_DOWN");
        interface_delay_ms(TIME_DOWN_D_MS);
        time_limiter(station_up,TIME_UP_MS,"STATION_UP");

    }
}