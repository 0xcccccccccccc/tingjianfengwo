#include <stdio.h>
#include <time.h>
#include "phy/radio_tx.h"
#include "phy/debug.h"
#include "phy/filters/fdacoefs_FIR_1000.h"
#include "phy/filters/fdacoefs_FIR_1500.h"
#include "phy/filters//fdacoefs_FIR_2000.h"
#include "phy/filters//fdacoefs_FIR_2500.h"
#include "phy/radio_rx.h"

uint8_t ch1_msg[]={0,0,0,SYNC_HEADER,0xff,0x55,0xf0,0,0};
uint8_t ch2_msg[]={0,0,0,SYNC_HEADER,'h','i','2',0,0};
uint8_t ch3_msg[]={0,0,0,SYNC_HEADER,'h','i','3',0,0};
uint8_t ch4_msg[]={0,0,0,SYNC_HEADER,'h','i','4',0,0};

void print_data(BIT_STREAM b,int32_t h){
    printf("%X\t",bit_stream_get_byte(b,h,0));
    printf("%X\t",bit_stream_get_byte(b,h,1));
    printf("%X\n",bit_stream_get_byte(b,h,2));
}

int main() {


    clock_t start,end;
    start = clock();


    SIGNAL signal1= radio_tx_generate_signal_from_bytes(1000.0, &ch1_msg,9);
    SIGNAL signal2= radio_tx_generate_signal_from_bytes(1500.0, &ch2_msg,9);
    SIGNAL signal3= radio_tx_generate_signal_from_bytes(2000.0, &ch3_msg,9);
    SIGNAL signal4= radio_tx_generate_signal_from_bytes(2500.0, &ch4_msg,9);
//    SIGNAL signal1=radio_tx_generate_signal_from_byte(1600.0,0x00);
//    SIGNAL signal2=radio_tx_generate_signal_from_byte(2300.0,0x55);
//    SIGNAL signal3=radio_tx_generate_signal_from_byte(3000.0,0x55);
//    SIGNAL signal4=radio_tx_generate_signal_from_byte(3700.0,0x55);

    signal_add_to(signal2,signal1);
    signal_add_to(signal3,signal1);
    signal_add_to(signal4,signal1);

    signal_normalize_to(signal1);

    signal_broadcast_add_to(-0.5,signal1);
    signal_broadcast_multiply_to(2.0,signal1);
    debug_signal_save_pcm_wav("b4_test.wav",signal1);


    struct SIGNAL_T kernel1600;
    kernel1600.data=&FIR_1000_B;
    kernel1600.boundary_type=SIGNAL_BOUNDARY_TYPE_FIXED;
    kernel1600.length=FIR_1000_BL;

    struct SIGNAL_T kernel2300;
    kernel2300.data=&FIR_1500_B;
    kernel2300.boundary_type=SIGNAL_BOUNDARY_TYPE_FIXED;
    kernel2300.length=FIR_1500_BL;

    struct SIGNAL_T kernel3000;
    kernel3000.data=&FIR_2000_B;
    kernel3000.boundary_type=SIGNAL_BOUNDARY_TYPE_FIXED;
    kernel3000.length=FIR_2000_BL;

    struct SIGNAL_T kernel3700;
    kernel3700.data=&FIR_2500_B;
    kernel3700.boundary_type=SIGNAL_BOUNDARY_TYPE_FIXED;
    kernel3700.length=FIR_2500_BL;


    signal1->boundary_type=SIGNAL_BOUNDARY_TYPE_ZERO_PADDING;
    SIGNAL ch1=signal_convolve(signal1,&kernel1600,SIGNAL_CONVOLVE_MODE_SAME);
    SIGNAL ch2=signal_convolve(signal1,&kernel2300,SIGNAL_CONVOLVE_MODE_SAME);
    SIGNAL ch3=signal_convolve(signal1,&kernel3000,SIGNAL_CONVOLVE_MODE_SAME);
    SIGNAL ch4=signal_convolve(signal1,&kernel3700,SIGNAL_CONVOLVE_MODE_SAME);

    ch1->boundary_type=SIGNAL_BOUNDARY_TYPE_ZERO_PADDING;
    ch2->boundary_type=SIGNAL_BOUNDARY_TYPE_ZERO_PADDING;
    ch3->boundary_type=SIGNAL_BOUNDARY_TYPE_ZERO_PADDING;
    ch4->boundary_type=SIGNAL_BOUNDARY_TYPE_ZERO_PADDING;


    SIGNAL ma1=signal_moving_average(ch1,20,SIGNAL_CONVOLVE_MODE_SAME);
    SIGNAL ma2=signal_moving_average(ch2,20,SIGNAL_CONVOLVE_MODE_SAME);
    SIGNAL ma3=signal_moving_average(ch3,20,SIGNAL_CONVOLVE_MODE_SAME);
    SIGNAL ma4=signal_moving_average(ch4,20,SIGNAL_CONVOLVE_MODE_SAME);
    debug_signal_save_pcm_wav("ch1.wav",ma1);
    debug_signal_save_pcm_wav("ch2.wav",ma2);
    debug_signal_save_pcm_wav("ch3.wav",ma3);
    debug_signal_save_pcm_wav("ch4.wav",ma4);

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


    BIT_STREAM sync_header=radio_tx_generate_sync_header();

    SIGNAL cor1=bit_stream_correlation(b1,sync_header,RADIO_RX_SEARCH_SYNC_HEADER_CORRELATION_STEP);
    SIGNAL cor2=bit_stream_correlation(b2,sync_header,RADIO_RX_SEARCH_SYNC_HEADER_CORRELATION_STEP);
    SIGNAL cor3=bit_stream_correlation(b3,sync_header,RADIO_RX_SEARCH_SYNC_HEADER_CORRELATION_STEP);
    SIGNAL cor4=bit_stream_correlation(b4,sync_header,RADIO_RX_SEARCH_SYNC_HEADER_CORRELATION_STEP);


    debug_signal_save_pcm_wav("ch1.wav",cor1);
    debug_signal_save_pcm_wav("ch2.wav",cor2);
    debug_signal_save_pcm_wav("ch3.wav",cor3);
    debug_signal_save_pcm_wav("ch4.wav",cor4);

    int h1=radio_rx_search_sync_header(b1,0);
    int h2=radio_rx_search_sync_header(b2,0);
    int h3=radio_rx_search_sync_header(b3,0);
    int h4=radio_rx_search_sync_header(b4,0);


    print_data(b1,h1);print_data(b2,h2);print_data(b3,h3);print_data(b4,h4);

//    debug_signal_save_pcm_wav("ch1.wav",cor1);
//    debug_signal_save_pcm_wav("ch2.wav",cor2);
//    debug_signal_save_pcm_wav("ch3.wav",cor3);
//    debug_signal_save_pcm_wav("ch4.wav",cor4);

    signal_del(signal1);
    signal_del(signal2);
    signal_del(signal3);
    signal_del(signal4);
    signal_del(ch1);
    signal_del(ch2);
    signal_del(ch3);
    signal_del(ch4);
    end = clock();
    double duration =(double)(end-start)/CLOCKS_PER_SEC;
    printf("%fms\n",duration); // 4.015
    return 0;
}
