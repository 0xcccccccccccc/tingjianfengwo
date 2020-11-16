//
// Created by ma on 2020/11/12.
//

#include "byte_t.h"

void bit_flip(uint8_t *target,uint8_t pos) {
    switch (pos) {
        case 0:
            ((union BYTE_T *)(target))->bit.b0=!((union BYTE_T *)(target))->bit.b0;
            break;
        case 1:
            ((union BYTE_T *)(target))->bit.b1=!((union BYTE_T *)(target))->bit.b1;
            break;
        case 2:
            ((union BYTE_T *)(target))->bit.b2=!((union BYTE_T *)(target))->bit.b2;
            break;
        case 3:
            ((union BYTE_T *)(target))->bit.b3=!((union BYTE_T *)(target))->bit.b3;
            break;
        case 4:
            ((union BYTE_T *)(target))->bit.b4=!((union BYTE_T *)(target))->bit.b4;
            break;
        case 5:
            ((union BYTE_T *)(target))->bit.b5=!((union BYTE_T *)(target))->bit.b5;
            break;
        case 6:
            ((union BYTE_T *)(target))->bit.b6=!((union BYTE_T *)(target))->bit.b6;
            break;
        case 7:
            ((union BYTE_T *)(target))->bit.b7=!((union BYTE_T *)(target))->bit.b7;
            break;
    }
}
void bit_set(uint8_t *target,uint8_t pos,uint8_t val) {
    switch (pos) {
        case 0:
            ((union BYTE_T *)(target))->bit.b0=val;
            break;
        case 1:
            ((union BYTE_T *)(target))->bit.b1=val;
            break;
        case 2:
            ((union BYTE_T *)(target))->bit.b2=val;
            break;
        case 3:
            ((union BYTE_T *)(target))->bit.b3=val;
            break;
        case 4:
            ((union BYTE_T *)(target))->bit.b4=val;
            break;
        case 5:
            ((union BYTE_T *)(target))->bit.b5=val;
            break;
        case 6:
            ((union BYTE_T *)(target))->bit.b6=val;
            break;
        case 7:
            ((union BYTE_T *)(target))->bit.b7=val;
            break;
    }
}
uint8_t bit_get(uint8_t *target,uint8_t pos) {
    switch (pos) {
        case 0:
            return ((union BYTE_T *)(target))->bit.b0;

        case 1:
            return ((union BYTE_T *)(target))->bit.b1;

        case 2:
            return ((union BYTE_T *)(target))->bit.b2;

        case 3:
            return ((union BYTE_T *)(target))->bit.b3;

        case 4:
            return ((union BYTE_T *)(target))->bit.b4;

        case 5:
            return ((union BYTE_T *)(target))->bit.b5;

        case 6:
            return ((union BYTE_T *)(target))->bit.b6;

        case 7:
            return ((union BYTE_T *)(target))->bit.b7;

    }
}

