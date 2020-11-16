//
// Created by ma on 2020/11/10.
//

#ifndef RADIO_BIT_STREAM_T_H
#define RADIO_BIT_STREAM_T_H

/* 1. 获取指定位数掩码 */
#include <stdint.h>
#include "../signal_t/signal_t.h"

#define BITS_MSK_GET(bitsoff, bits) (((0x01 << (bits)) - 1) << (bitsoff))
/* 2. 清除指定N BITS */
#define BITS_CLR(var,bitsoff,bits) ((var) & (~(BITS_MSK_GET(bitsoff,bits))))
/* 3. 获取指定 N BITS 值 */
#define BITS_GET(var,bitsoff,bits) ((var) & (BITS_MSK_GET(bitsoff,bits)))
/* 4. 获取指定N BITS 值并右对齐 */
#define BITS_RIGHT_JUST_GET(var,bitsoff,bits) \
((BITS_GET(var,bitsoff,bits) >> (bitsoff)) & BITS_MSK_GET(0,bits))
/* 5. 给变量指定BITS赋值 */
#define BITS_VAL_SET(var,bitsval,bitsoff,bits) \
(var = BITS_CLR(var,bitsoff,bits) | (((bitsval) << (bitsoff)) & BITS_MSK_GET(0,bits)))
static enum {
    BIT_STREAM_BOUNDARY_FIXED,
    BIT_STREAM_BOUNDARY_ZERO_PADDING,
    BIT_STREAM_BOUNDARY_LOOP
} BIT_STREAM_BOUNDARY_TYPE;
struct BIT_STREAM_T{
    uint8_t* data;
    int32_t length;
    uint8_t boundary_type;
};
typedef struct BIT_STREAM_T* BIT_STREAM;

SIGNAL bit_stream_correlation(BIT_STREAM src,BIT_STREAM target,uint32_t cmp_step);
uint8_t bit_stream_get(BIT_STREAM bit_stream,int32_t pos);
int8_t bit_stream_set(BIT_STREAM bit_stream,int32_t pos,uint8_t val);
BIT_STREAM bit_stream_new(int32_t length,uint8_t boundary_type);
int8_t bit_stream_del(BIT_STREAM bit_stream);
uint8_t bit_stream_get_digit(BIT_STREAM src,int32_t base,int32_t pos);
uint8_t bit_stream_get_byte(BIT_STREAM src,int32_t base,int32_t pos);

#endif //RADIO_BIT_STREAM_T_H
