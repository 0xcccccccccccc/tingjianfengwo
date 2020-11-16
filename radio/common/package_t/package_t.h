//
// Created by ma on 2020/11/12.
//

#ifndef RADIO_PACKAGE_T_H
#define RADIO_PACKAGE_T_H
#include "stdint.h"
#include "../signal_t/signal_t.h"
#include "../bit_stream_t/bit_stream_t.h"

typedef struct{uint8_t data[3];} PACKAGE;
static const PACKAGE NULL_PACKAGE={0};
SIGNAL package_to_signal(int channel,PACKAGE package);
PACKAGE package_new(uint8_t d1,uint8_t d2);
PACKAGE package_decode_from_bit_stream(BIT_STREAM bit_stream,int32_t base);
uint8_t package_is_null(PACKAGE p);
PACKAGE* package_alloc_decode_from_bit_stream(BIT_STREAM bit_stream,int32_t base);
uint8_t package_network_get_src(PACKAGE p);
uint8_t package_network_get_dst(PACKAGE p);
#endif //RADIO_PACKAGE_T_H
