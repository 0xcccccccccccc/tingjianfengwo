//
// Created by ma on 2020/11/10.
//

#ifndef RADIO_SIGNAL_T_H
#define RADIO_SIGNAL_T_H
#include <stdint.h>

//typedef double RADIO_SIGNAL_SCALAR_TYPE;
//#define RADIO_SIGNAL_SCALAR_MIN 2.22507e-308
//#define RADIO_SIGNAL_SCALAR_MAX 1.79769e+308
//#define RADIO_SIGNAL_SCALAR_ZERO 0.0
typedef float RADIO_SIGNAL_SCALAR_TYPE;
#define RADIO_SIGNAL_SCALAR_MAX 3.4028235e38
#define RADIO_SIGNAL_SCALAR_MIN -3.4028235e38
#define RADIO_SIGNAL_SCALAR_ZERO 0.0

static enum{
    SIGNAL_CONVOLVE_MODE_SAME,
    SIGNAL_CONVOLVE_MODE_FULL,
    SIGNAL_CONVOLVE_MODE_SHORT,

}SIGNAL_CONVOLVE_MODE;

static enum{
    SIGNAL_BOUNDARY_TYPE_FIXED,
    SIGNAL_BOUNDARY_TYPE_ZERO_PADDING,
    SIGNAL_BOUNDARY_TYPE_LOOP
}SIGNAL_BOUNDARY_TYPE;



struct SIGNAL_T{
    int32_t length;
    RADIO_SIGNAL_SCALAR_TYPE *data;
    uint8_t boundary_type;
};
typedef struct SIGNAL_T* SIGNAL;

SIGNAL signal_new(int32_t length,RADIO_SIGNAL_SCALAR_TYPE init_val,uint8_t signal_boundary_type);
uint8_t signal_del(SIGNAL signal);
uint8_t signal_set(SIGNAL signal,uint32_t pos,RADIO_SIGNAL_SCALAR_TYPE val);
RADIO_SIGNAL_SCALAR_TYPE signal_get(SIGNAL signal,uint32_t pos);

uint8_t signal_add_to(SIGNAL src,SIGNAL dst);
uint8_t signal_broadcast_add_to(RADIO_SIGNAL_SCALAR_TYPE src,SIGNAL dst);
SIGNAL signal_broadcast_add(RADIO_SIGNAL_SCALAR_TYPE a,SIGNAL b);
SIGNAL signal_add(SIGNAL a,SIGNAL b);

uint8_t signal_multilpy_to(SIGNAL src,SIGNAL dst);
uint8_t signal_broadcast_multiply_to(RADIO_SIGNAL_SCALAR_TYPE src,SIGNAL dst);
SIGNAL signal_broadcast_multiply(RADIO_SIGNAL_SCALAR_TYPE a,SIGNAL b);
SIGNAL signal_multiply(SIGNAL a,SIGNAL b);

uint8_t signal_normalize_to(SIGNAL signal);
SIGNAL signal_normalize(SIGNAL signal);
SIGNAL signal_concat(SIGNAL left,SIGNAL right);
uint8_t signal_concat_to(SIGNAL src,SIGNAL dst);
RADIO_SIGNAL_SCALAR_TYPE signal_max_scalar(SIGNAL signal);
RADIO_SIGNAL_SCALAR_TYPE signal_min_scalar(SIGNAL signal);
uint32_t signal_argmax(SIGNAL signal);
uint32_t signal_argmin(SIGNAL signal);

int8_t signal_convolve_to(SIGNAL src,SIGNAL kernel,SIGNAL dst);
SIGNAL signal_convolve(SIGNAL src,SIGNAL kernel,uint8_t convolve_mode);
int8_t signal_moving_average_to(SIGNAL src,int32_t window_length,SIGNAL dst);
SIGNAL signal_moving_average(SIGNAL src,int32_t window_length,uint8_t convolve_mode);
void signal_write_pcm16_to_buffer(int16_t* buffer,SIGNAL signal,int32_t bias);
SIGNAL signal_create_from_pcm16(int16_t* buffer,int32_t length);
SIGNAL signal_copy(SIGNAL signal);
SIGNAL signal_resample(SIGNAL signal,double original_fs,double target_fs);
#endif //RADIO_SIGNAL_T_H
