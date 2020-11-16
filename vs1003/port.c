//
// Created by ma on 2020/11/15.
//
#include <stdint.h>
#include <stdio.h>
#include <rtthread.h>



static uint8_t r_byte(uint8_t x)
{
    static const uint8_t table[] =
            {
                    0x00, 0x80, 0x40, 0xc0, 0x20, 0xa0, 0x60, 0xe0,
                    0x10, 0x90, 0x50, 0xd0, 0x30, 0xb0, 0x70, 0xf0,
                    0x08, 0x88, 0x48, 0xc8, 0x28, 0xa8, 0x68, 0xe8,
                    0x18, 0x98, 0x58, 0xd8, 0x38, 0xb8, 0x78, 0xf8,
                    0x04, 0x84, 0x44, 0xc4, 0x24, 0xa4, 0x64, 0xe4,
                    0x14, 0x94, 0x54, 0xd4, 0x34, 0xb4, 0x74, 0xf4,
                    0x0c, 0x8c, 0x4c, 0xcc, 0x2c, 0xac, 0x6c, 0xec,
                    0x1c, 0x9c, 0x5c, 0xdc, 0x3c, 0xbc, 0x7c, 0xfc,
                    0x02, 0x82, 0x42, 0xc2, 0x22, 0xa2, 0x62, 0xe2,
                    0x12, 0x92, 0x52, 0xd2, 0x32, 0xb2, 0x72, 0xf2,
                    0x0a, 0x8a, 0x4a, 0xca, 0x2a, 0xaa, 0x6a, 0xea,
                    0x1a, 0x9a, 0x5a, 0xda, 0x3a, 0xba, 0x7a, 0xfa,
                    0x06, 0x86, 0x46, 0xc6, 0x26, 0xa6, 0x66, 0xe6,
                    0x16, 0x96, 0x56, 0xd6, 0x36, 0xb6, 0x76, 0xf6,
                    0x0e, 0x8e, 0x4e, 0xce, 0x2e, 0xae, 0x6e, 0xee,
                    0x1e, 0x9e, 0x5e, 0xde, 0x3e, 0xbe, 0x7e, 0xfe,
                    0x01, 0x81, 0x41, 0xc1, 0x21, 0xa1, 0x61, 0xe1,
                    0x11, 0x91, 0x51, 0xd1, 0x31, 0xb1, 0x71, 0xf1,
                    0x09, 0x89, 0x49, 0xc9, 0x29, 0xa9, 0x69, 0xe9,
                    0x19, 0x99, 0x59, 0xd9, 0x39, 0xb9, 0x79, 0xf9,
                    0x05, 0x85, 0x45, 0xc5, 0x25, 0xa5, 0x65, 0xe5,
                    0x15, 0x95, 0x55, 0xd5, 0x35, 0xb5, 0x75, 0xf5,
                    0x0d, 0x8d, 0x4d, 0xcd, 0x2d, 0xad, 0x6d, 0xed,
                    0x1d, 0x9d, 0x5d, 0xdd, 0x3d, 0xbd, 0x7d, 0xfd,
                    0x03, 0x83, 0x43, 0xc3, 0x23, 0xa3, 0x63, 0xe3,
                    0x13, 0x93, 0x53, 0xd3, 0x33, 0xb3, 0x73, 0xf3,
                    0x0b, 0x8b, 0x4b, 0xcb, 0x2b, 0xab, 0x6b, 0xeb,
                    0x1b, 0x9b, 0x5b, 0xdb, 0x3b, 0xbb, 0x7b, 0xfb,
                    0x07, 0x87, 0x47, 0xc7, 0x27, 0xa7, 0x67, 0xe7,
                    0x17, 0x97, 0x57, 0xd7, 0x37, 0xb7, 0x77, 0xf7,
                    0x0f, 0x8f, 0x4f, 0xcf, 0x2f, 0xaf, 0x6f, 0xef,
                    0x1f, 0x9f, 0x5f, 0xdf, 0x3f, 0xbf, 0x7f, 0xff,
            };

    return table[x];
}

enum{
    NOT_SYNC=0,
    FIRST_BYTE=1,
    SECOND_BYTE=2,
    THIRD_BYTE=3,
    DONE=4
};

//const uint8_t sync_header=0b01110010;
const uint8_t sync_header=0b01001110;

float op_iir_1600(float input){
#define NZEROS  14
#define NPOLES  14
#define GAIN  8.254404392e+09
    static float xv[NZEROS+1], yv[NPOLES+1];
    xv[0] = xv[1]; xv[1] = xv[2]; xv[2] = xv[3]; xv[3] = xv[4]; xv[4] = xv[5]; xv[5] = xv[6]; xv[6] = xv[7]; xv[7] = xv[8]; xv[8] = xv[9]; xv[9] = xv[10]; xv[10] = xv[11]; xv[11] = xv[12]; xv[12] = xv[13]; xv[13] = xv[14];
    xv[14] = input / GAIN;
    yv[0] = yv[1]; yv[1] = yv[2]; yv[2] = yv[3]; yv[3] = yv[4]; yv[4] = yv[5]; yv[5] = yv[6]; yv[6] = yv[7]; yv[7] = yv[8]; yv[8] = yv[9]; yv[9] = yv[10]; yv[10] = yv[11]; yv[11] = yv[12]; yv[12] = yv[13]; yv[13] = yv[14];
    yv[14] =   (xv[14] - xv[0]) + 7 * (xv[2] - xv[12]) + 21 * (xv[10] - xv[4])
               + 35 * (xv[6] - xv[8])
               + ( -0.7025269855 * yv[0]) + (  3.1183241432 * yv[1])
               + (-11.0982855640 * yv[2]) + ( 25.9285198960 * yv[3])
               + (-51.4343747050 * yv[4]) + ( 79.5326851830 * yv[5])
               + (-106.9395645100 * yv[6]) + (117.2161645200 * yv[7])
               + (-112.4708924700 * yv[8]) + ( 87.9731834420 * yv[9])
               + (-59.8358182730 * yv[10]) + ( 31.7243385000 * yv[11])
               + (-14.2815867190 * yv[12]) + (  4.2204266643 * yv[13]);

    return yv[14];
#undef NZEROS
#undef NPOLES
#undef GAIN
}
float op_int_5(float input){

    static float xv[5];
    xv[0] = xv[1]; xv[1] = xv[2]; xv[2] = xv[3]; xv[3] = xv[4];
    xv[4]=input;

    return (xv[0]+xv[1]+xv[2]+xv[3]+xv[4])/5.0;
}
float op_abs(float input){
    //printf("%f\n",fabsf(input));
    return fabsf(input);
}
uint8_t op_bin(float input){
    //printf("%d\n",(uint8_t)(input>0.125));
    return (uint8_t)(input>0.65);
}
float op_int_4(float input){
    static float xv[4];
    xv[0] = xv[1]; xv[1] = xv[2]; xv[2] = xv[3];
    xv[3]=input;
    return (xv[0]+xv[1]+xv[2]+xv[3])/4.0;
}
void op_ch2_byte_received(uint8_t input){
    rt_kprintf("CH2:%x\n",input);
}

void op_ch2_mod5_sync(uint8_t input){
    static uint8_t b[5]={0,0,0,0,0};
    static uint8_t b_sync[5]={0,0,0,0,0};
    static uint8_t nbits_received=0;
    static uint8_t target_b=0xff;
    static uint8_t status=NOT_SYNC;

    static int ctr=0;

    b[ctr]<<=1;
    b[ctr]=b[ctr]|(input&0x01);
    //printf("%d\t",input);
    ctr++;

    if(ctr==5){
        //printf("\n");
        ctr=0;
        if(status==NOT_SYNC){
            // check sync
            int sync_ctr=0;
            for(int i=0;i<5;i++){
                b_sync[i]=((b[i]&0x7f)==sync_header);
                if(b_sync[i]){
                    sync_ctr++;
                    target_b=i;
                }
            }
            if(sync_ctr>2){
                // synced
                status=FIRST_BYTE;
            }
        } else{
            nbits_received++;
            if(nbits_received==8){
                op_ch2_byte_received(r_byte(b[target_b]));
                nbits_received=0;
                status++;
                if(status==DONE){
                    status=NOT_SYNC;
                    nbits_received=0;
                    target_b=0xff;
                }
            }
        }
    }
}



void op_ch2_resample_25(uint8_t input){
    static uint8_t ctr=0;
    if(ctr%25==0){
        rt_kprintf("%f\n",(float )input);
        op_ch2_mod5_sync(input);

    }
    ctr=(ctr%25+1);
}

void radio_rx_decode_realtime(float signal){
    op_ch2_resample_25(op_bin(op_int_5(op_abs(op_iir_1600(signal)))));
}