//
// Created by ma on 2020/11/12.
//

#ifndef RADIO_BYTE_T_H
#define RADIO_BYTE_T_H
#include <stdint.h>
union BYTE_T{
    uint8_t byte;
    struct {
        uint8_t b0:1;
        uint8_t b1:1;
        uint8_t b2:1;
        uint8_t b3:1;
        uint8_t b4:1;
        uint8_t b5:1;
        uint8_t b6:1;
        uint8_t b7:1;
    }bit;
};
void bit_flip(uint8_t *target,uint8_t pos);
void bit_set(uint8_t *target,uint8_t pos,uint8_t val);
uint8_t bit_get(uint8_t *target,uint8_t pos);
#endif //RADIO_BYTE_T_H
