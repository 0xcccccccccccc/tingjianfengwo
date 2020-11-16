//
// Created by ma on 2020/11/10.
//

#include "bit_stream_t.h"
#include "../signal_t/signal_t.h"
#include "../config.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>


// BIT_STREAM was encoded by little-endian
//                                         raw_data
//          0         1         2         3         4         5         6         7
//        (LSB)                                                                  (MSB)

BIT_STREAM bit_stream_new(int32_t length,uint8_t boundary_type){
    BIT_STREAM bit_stream=malloc(sizeof(struct BIT_STREAM_T));
    bit_stream->boundary_type=boundary_type;
    bit_stream->data=malloc(sizeof(u_int8_t)*length);
    bit_stream->length=length;
    return bit_stream;
}
int8_t bit_stream_del(BIT_STREAM bit_stream){
    free(bit_stream->data);
    free(bit_stream);
}

int8_t bit_stream_set(BIT_STREAM bit_stream,int32_t pos,uint8_t val){
    switch(bit_stream->boundary_type){
        case BIT_STREAM_BOUNDARY_FIXED:
            if(bit_stream->length>pos)
                bit_stream->data[pos]=val;
            else return -1;
            break;
        case BIT_STREAM_BOUNDARY_LOOP:
            bit_stream->data[pos%(bit_stream->length)]=val;
            break;
        case BIT_STREAM_BOUNDARY_ZERO_PADDING:
            if(bit_stream->length>pos)
                bit_stream->data[pos]=val;
            else return -1;
            break;
    }
}
uint8_t bit_stream_get(BIT_STREAM bit_stream,int32_t pos){
    switch(bit_stream->boundary_type){
        case BIT_STREAM_BOUNDARY_FIXED:
            if(bit_stream->length>pos)
                return bit_stream->data[pos];
            else{
                printf("BitStream write error.");
            }

            break;
        case BIT_STREAM_BOUNDARY_LOOP:
            return bit_stream->data[pos%(bit_stream->length)];
            break;
        case BIT_STREAM_BOUNDARY_ZERO_PADDING:
            if(bit_stream->length>pos)
                return bit_stream->data[pos];
            else return 0;
            break;
    }
}
static BIT_STREAM bit_stream_copy(BIT_STREAM bit_stream){
    BIT_STREAM new_bit_stream=bit_stream_new(bit_stream->length,bit_stream->boundary_type);
    for(int i=0;i<bit_stream->length;i++){
        new_bit_stream->data[i]=bit_stream->data[i];
    }
    return new_bit_stream;
}

SIGNAL bit_stream_correlation(BIT_STREAM src,BIT_STREAM target,uint32_t cmp_step){
    BIT_STREAM bit_stream=bit_stream_copy(src);
    bit_stream->boundary_type=BIT_STREAM_BOUNDARY_ZERO_PADDING;
    SIGNAL signal=signal_new(bit_stream->length,0.0,SIGNAL_BOUNDARY_TYPE_FIXED);
    for(int bias=0;bias<bit_stream->length;bias++){
        for(int i=0;i<(target->length-cmp_step);i+=cmp_step){
            signal->data[bias]+=(float)(bit_stream_get(bit_stream,i+bias)==bit_stream_get(target,i));
        }
    }
    bit_stream_del(bit_stream);
    return signal;
}

uint8_t bit_stream_get_digit(BIT_STREAM src,int32_t base,int32_t pos){
    int32_t bias=base+pos*(TONE_LENGTH+TONE_SPACING_LENGTH);
    int32_t sum=0;
    //printf("get bit from :%d to %d\n",bias,bias+TONE_LENGTH);
    for(int i=0;i<TONE_LENGTH;i++){
        sum+=bit_stream_get(src,bias+i);
    }
    return sum>0.5*TONE_LENGTH;
}

uint8_t bit_stream_get_byte(BIT_STREAM src,int32_t base,int32_t pos){
    uint8_t ret=0;
    for(int i=0;i<7;i++){
        ret|=bit_stream_get_digit(src,base+pos*8*(TONE_LENGTH+TONE_SPACING_LENGTH),7-i);
        ret<<=1;
    }
    ret|=bit_stream_get_digit(src,base+pos*8*(TONE_LENGTH+TONE_SPACING_LENGTH),0);
    return ret;
}