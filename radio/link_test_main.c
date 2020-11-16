//
// Created by ma on 2020/11/12.
//

#include <stdio.h>
#include <math.h>
#include "common/package_t/package_t.h"
#include "phy/debug.h"
#include "phy/filters/fdacoefs_FIR_1000.h"
#include "common/bit_stream_t/bit_stream_t.h"
#include "phy/radio_rx.h"

int main(){
//SIGNAL signal=package_to_signal(0,package_new(0xfe,0x55));
SIGNAL signal=debug_signal_load_pcm_wav("test_2138.wav");
debug_signal_save_pcm_wav("origin_signal.wav",signal);
signal->boundary_type=SIGNAL_BOUNDARY_TYPE_ZERO_PADDING;
SIGNAL filted=signal_convolve(signal,&FIR_KERNEL_1000,SIGNAL_CONVOLVE_MODE_SAME);
signal_del(signal);

filted->boundary_type=SIGNAL_BOUNDARY_TYPE_ZERO_PADDING;
SIGNAL dc=signal_moving_average(filted,20,SIGNAL_CONVOLVE_MODE_SAME);
signal_del(filted);


BIT_STREAM bin=radio_rx_binarize(dc,0.5);
bin->boundary_type=BIT_STREAM_BOUNDARY_ZERO_PADDING;
signal_del(dc);

int base=radio_rx_search_sync_header(bin,0);
PACKAGE package=package_decode_from_bit_stream(bin,base);
bit_stream_del(bin);



printf("%x %x \n",package.data[0],package.data[1]);



}