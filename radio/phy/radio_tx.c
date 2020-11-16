//
// Created by ma on 2020/11/10.
//

#include "radio_tx.h"
#include "../common/signal_t/signal_t.h"
#include <math.h>
#include "../common/config.h"
#include "../common/bit_stream_t/bit_stream_t.h"
// raw_data was encoded by little-endian
//                                         raw_data
//          0         1         2         3         4         5         6         7
// (First bit encoded)                                                    (Last bit encoded)
//        (LSB)                                                                  (MSB)

SIGNAL radio_tx_generate_signal_from_byte(double target_freq, uint8_t raw_data){
    SIGNAL signal=signal_new(8*TONE_LENGTH+8*TONE_SPACING_LENGTH,RADIO_SIGNAL_SCALAR_ZERO,SIGNAL_BOUNDARY_TYPE_FIXED);
    for(int i=0;i<8;i++){
        uint8_t current_bit=(raw_data>>i)&0x1;
        int current_base=i*(TONE_SPACING_LENGTH+TONE_LENGTH);
        if(current_bit)
        for(int n=0;n<TONE_LENGTH;n++){
            signal_set(signal,current_base+n,sin(2.0*PI*(double)n*target_freq/(double)FS)/**HAMMING_WINDOW_800[n]*/);
            //signal->data[current_base+n]=sin(2.0*PI*n*target_freq/(double)FS);
        }
    }
    return signal;
}

SIGNAL radio_tx_generate_signal_from_bytes(double target_freq, uint8_t* raw_data,uint32_t data_length){
    SIGNAL signal=signal_new((8*TONE_LENGTH+8*TONE_SPACING_LENGTH)*data_length,RADIO_SIGNAL_SCALAR_ZERO,SIGNAL_BOUNDARY_TYPE_FIXED);
    for(int d=0;d<data_length;d++)
    for(int i=0;i<8;i++){
        uint8_t current_bit=((raw_data[d])>>i)&0x1;
        int current_base=i*(TONE_SPACING_LENGTH+TONE_LENGTH)+d*(8*TONE_LENGTH+8*TONE_SPACING_LENGTH);
        if(current_bit)
            for(int n=0;n<TONE_LENGTH;n++){
                signal_set(signal,current_base+n,sin(2.0*PI*(double)n*target_freq/(double)FS)/**HAMMING_WINDOW_800[n]*/);
                //signal->data[current_base+n]=sin(2.0*PI*n*target_freq/(double)FS);
            }
    }
    return signal;
}
const uint8_t SYNCHEADER_STREAM[7]={1, 0, 0, 1, 1, 1, 0};
BIT_STREAM radio_tx_generate_sync_header(){
    BIT_STREAM bit_stream=bit_stream_new(8*TONE_LENGTH+8*TONE_SPACING_LENGTH,BIT_STREAM_BOUNDARY_FIXED);
    for(int i=0;i<8;i++){
        uint8_t current_bit= (SYNCHEADER_STREAM[i]) & 0x1;
        int current_base=i*(TONE_SPACING_LENGTH+TONE_LENGTH);
        if(current_bit)
            for(int n=0;n<TONE_LENGTH;n++){
                bit_stream_set(bit_stream,current_base+n,1);
            }
    }
    return bit_stream;
}