//
// Created by ma on 2020/11/10.
//
#include <stdlib.h>
#include <stdio.h>
#include "signal_t.h"
SIGNAL signal_new(int32_t length,RADIO_SIGNAL_SCALAR_TYPE init_val,uint8_t signal_boundary_type){
    SIGNAL signal=malloc(sizeof(struct SIGNAL_T));
    signal->length=length;
    signal->boundary_type=signal_boundary_type;
    signal->data=malloc(sizeof(RADIO_SIGNAL_SCALAR_TYPE)*length);
    for(int32_t i=0;i<length;i++){
        signal->data[i]=init_val;
    }
    return signal;
}
uint8_t signal_del(SIGNAL signal){
    free(signal->data);
    free(signal);
}
uint8_t signal_set(SIGNAL signal,uint32_t pos,RADIO_SIGNAL_SCALAR_TYPE val){
    switch(signal->boundary_type){
        case SIGNAL_BOUNDARY_TYPE_FIXED:
            if(pos<(signal->length)){
                *((RADIO_SIGNAL_SCALAR_TYPE *)(signal->data)+pos)=val;
            }
            else{
                printf("Invalid input index");
            }
            break;
        case SIGNAL_BOUNDARY_TYPE_LOOP:
            if(pos<(signal->length)){
                *((RADIO_SIGNAL_SCALAR_TYPE *)(signal->data)+pos)=val;
            }
            else{
                *((RADIO_SIGNAL_SCALAR_TYPE *)(signal->data)+(pos%signal->length))=val;
            }
            break;
        case SIGNAL_BOUNDARY_TYPE_ZERO_PADDING:
            if(pos<(signal->length)){
                *((RADIO_SIGNAL_SCALAR_TYPE *)(signal->data)+pos)=val;
            }
            else{
                printf("Invalid input index");
            }
            break;
    }



}
RADIO_SIGNAL_SCALAR_TYPE signal_get(SIGNAL signal,uint32_t pos){
    switch(signal->boundary_type){
        case SIGNAL_BOUNDARY_TYPE_FIXED:
            if(pos<(signal->length)){
                return *((RADIO_SIGNAL_SCALAR_TYPE *)(signal->data)+pos);
            }
            else{
                printf("Signal invalid input index");
                return RADIO_SIGNAL_SCALAR_ZERO;
            }
            break;
        case SIGNAL_BOUNDARY_TYPE_LOOP:
            if(pos<(signal->length)){
                return *((RADIO_SIGNAL_SCALAR_TYPE *)(signal->data)+pos);
            }
            else{
                return *((RADIO_SIGNAL_SCALAR_TYPE *)(signal->data)+(pos%signal->length));
            }
            break;
        case SIGNAL_BOUNDARY_TYPE_ZERO_PADDING:
            if(pos<(signal->length)){
                return *((RADIO_SIGNAL_SCALAR_TYPE *)(signal->data)+pos);
            }
            else{
                return RADIO_SIGNAL_SCALAR_ZERO;
            }
            break;
    }

}
SIGNAL signal_copy(SIGNAL signal){
    SIGNAL new_signal=signal_new(signal->length,RADIO_SIGNAL_SCALAR_ZERO,SIGNAL_BOUNDARY_TYPE_FIXED);
    for(int i=0;i<signal->length;i++){
        new_signal->data[i]=signal->data[i];
    }
    return new_signal;
}
uint8_t signal_add_to(SIGNAL src,SIGNAL dst){
    for(int32_t i=0;i<dst->length;i++){
        dst->data[i]+=src->data[i];
    }
}
uint8_t signal_broadcast_add_to(RADIO_SIGNAL_SCALAR_TYPE src,SIGNAL dst){
    for(int32_t i=0;i<dst->length;i++){
        dst->data[i]+=src;
    }
}

SIGNAL signal_add(SIGNAL a,SIGNAL b){
    SIGNAL signal=signal_copy(a);
    signal_add_to(b,signal);
    return signal;
}

SIGNAL signal_broadcast_add(RADIO_SIGNAL_SCALAR_TYPE a,SIGNAL b){
    SIGNAL signal=signal_copy(b);
    signal_broadcast_add_to(a,signal);
    return signal;
}

uint8_t signal_multilpy_to(SIGNAL src,SIGNAL dst){
    for(int32_t i=0;i<dst->length;i++){
        dst->data[i]*=src->data[i];
    }
}
uint8_t signal_broadcast_multiply_to(RADIO_SIGNAL_SCALAR_TYPE src,SIGNAL dst){
    for(int32_t i=0;i<dst->length;i++){
        dst->data[i]*=src;
    }
}
SIGNAL signal_broadcast_multiply(RADIO_SIGNAL_SCALAR_TYPE a,SIGNAL b){
    SIGNAL signal=signal_copy(b);
    signal_broadcast_multiply_to(a,signal);
    return signal;
}
SIGNAL signal_multiply(SIGNAL a,SIGNAL b){
    SIGNAL signal=signal_copy(a);
    signal_multilpy_to(b,signal);
    return signal;
}

uint8_t signal_normalize_to(SIGNAL signal){
    RADIO_SIGNAL_SCALAR_TYPE max_val=signal_max_scalar(signal);
    RADIO_SIGNAL_SCALAR_TYPE min_val=signal_min_scalar(signal);
    RADIO_SIGNAL_SCALAR_TYPE len_val=max_val-min_val;
    for(int32_t i=0;i<signal->length;i++){
        signal->data[i]=(signal->data[i]-min_val)/len_val;
    }
}

SIGNAL signal_normalize(SIGNAL signal){
    SIGNAL new_signal=signal_copy(signal);
    signal_normalize_to(new_signal);
    return new_signal;
}
uint8_t signal_concat_to(SIGNAL src,SIGNAL dst){
    uint32_t new_length=src->length+dst->length;
    int ret=realloc(dst->data,new_length);
    if(ret==0) return 1;
    for(int i=0;i<src->length;i++){
        dst->data[dst->length+i]=src->data[i];
    }
    return 0;

}
SIGNAL signal_concat(SIGNAL left,SIGNAL right){
    uint32_t new_length=left->length+right->length;
    SIGNAL new_signal=signal_new(new_length,RADIO_SIGNAL_SCALAR_ZERO,SIGNAL_BOUNDARY_TYPE_FIXED);
    for(int i=0;i<left->length;i++){
        new_signal->data[i]=left->data[i];
    }
    for(int i=0;i<right->length;i++){
        new_signal->data[left->length+i]=right->data[i];
    }
    return new_signal;
}
RADIO_SIGNAL_SCALAR_TYPE signal_max_scalar(SIGNAL signal){
    RADIO_SIGNAL_SCALAR_TYPE max_val=RADIO_SIGNAL_SCALAR_MIN;
    for(int32_t i=0;i<signal->length;i++){
        if(signal->data[i]>max_val){
            max_val=signal->data[i];
        }
    }
    return max_val;
}
RADIO_SIGNAL_SCALAR_TYPE signal_min_scalar(SIGNAL signal){
    RADIO_SIGNAL_SCALAR_TYPE min_val=RADIO_SIGNAL_SCALAR_MAX;
    for(int32_t i=0;i<signal->length;i++){
        if(signal->data[i]<min_val){
            min_val=signal->data[i];
        }
    }
    return min_val;
}

int8_t signal_convolve_to(SIGNAL src,SIGNAL kernel,SIGNAL dst)
{
    for (size_t i = 0; i < dst->length; ++i)
    {
        double sum = 0;
        for (size_t j = 0; j < kernel->length; ++j)
            sum += signal_get(src,i+j)* signal_get(kernel,kernel->length-1-j);
        signal_set(dst,i,sum);
    }
}

SIGNAL signal_convolve(SIGNAL src,SIGNAL kernel,uint8_t convolve_mode){
    SIGNAL dst=NULL;
    switch (convolve_mode) {
        case SIGNAL_CONVOLVE_MODE_SAME:

            dst=signal_new(src->length,0.0,SIGNAL_BOUNDARY_TYPE_FIXED);
            if(src->boundary_type==SIGNAL_BOUNDARY_TYPE_ZERO_PADDING)
            signal_convolve_to(src,kernel,dst);
            else{
                printf("SIGNAL_CONVOLVE_MODE_SAME is not compatible to non SIGNAL_BOUNDARY_TYPE_ZERO_PADDING type signal.");
            }
            return dst;
            break;
        default:
            printf("Not implentmented yet!");

    }

}


int8_t signal_moving_average_to(SIGNAL src,int32_t window_length,SIGNAL dst)
{
    for (size_t i = 0; i < dst->length; ++i)
    {
        double sum = 0;
        for (size_t j = 0; j < window_length; ++j)
            sum += signal_get(src,i+j)*signal_get(src,i+j);
        signal_set(dst,i,sum);
    }
}

SIGNAL signal_moving_average(SIGNAL src,int32_t window_length,uint8_t convolve_mode){
    SIGNAL dst=NULL;
    switch (convolve_mode) {
        case SIGNAL_CONVOLVE_MODE_SAME:

            dst=signal_new(src->length,0.0,SIGNAL_BOUNDARY_TYPE_FIXED);
            if(src->boundary_type==SIGNAL_BOUNDARY_TYPE_ZERO_PADDING)
                signal_moving_average_to(src,window_length,dst);
            else{
                printf("SIGNAL_CONVOLVE_MODE_SAME is not compatible to non SIGNAL_BOUNDARY_TYPE_ZERO_PADDING type signal.");
            }
            return dst;
            break;
        default:
            printf("Not implentmented yet!");

    }

}
uint32_t signal_argmax(SIGNAL signal){
    RADIO_SIGNAL_SCALAR_TYPE max_val=RADIO_SIGNAL_SCALAR_MIN;
    int32_t max_ind=0;
    for(int32_t i=0;i<signal->length;i++){
        if(signal->data[i]>max_val){
            max_val=signal->data[i];
            max_ind=i;
        }
    }
    return max_ind;
}
uint32_t signal_argmin(SIGNAL signal){
    RADIO_SIGNAL_SCALAR_TYPE min_val=RADIO_SIGNAL_SCALAR_MAX;
    int32_t min_ind=0;
    for(int32_t i=0;i<signal->length;i++){
        if(signal->data[i]<min_val){
            min_val=signal->data[i];
            min_ind=i;
        }
    }
    return min_ind;
}

void signal_write_pcm16_to_buffer(int16_t* buffer,SIGNAL signal,int32_t bias){
    for(uint32_t i=0;i<signal->length;i++){
        buffer[bias+i]=(int16_t)signal_get(signal,i);
    }
}

SIGNAL signal_create_from_pcm16(int16_t* buffer,int32_t length){
    SIGNAL signal=signal_new(length,0.0,SIGNAL_BOUNDARY_TYPE_FIXED);
    for(int32_t i=0;i<length;i++){
        signal->data[i]=(RADIO_SIGNAL_SCALAR_TYPE)buffer[i];
    }
    return signal;
}
SIGNAL signal_resample(SIGNAL signal,double original_fs,double target_fs){
    double ratio=target_fs/original_fs;
    SIGNAL new_signal=signal_new(signal->length*ratio,0.0,SIGNAL_BOUNDARY_TYPE_FIXED);
    for(int32_t i=0;i<new_signal->length;i++){
        new_signal->data[i]=signal->data[(int32_t)(i/ratio)];
    }
    return new_signal;
}