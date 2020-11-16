//
// Created by ma on 2020/11/12.
//

#ifndef RADIO_HAMMING_H
#define RADIO_HAMMING_H
#include <stdint.h>
#ifndef null
#define null ((void*) 0)
#endif

#define UNCORRECTABLE	0xFF
#define ERROR_IN_PARITY	0xFE
#define NO_ERROR		0x00

static const uint8_t _hammingCorrect128Syndrome[16]  =
        {
                NO_ERROR,			// 0
                ERROR_IN_PARITY,	// 1
                ERROR_IN_PARITY,	// 2
                0x01,				// 3
                ERROR_IN_PARITY,	// 4
                0x02,				// 5
                0x04,				// 6
                0x08,				// 7
                ERROR_IN_PARITY,	// 8
                0x10,				// 9
                0x20,				// 10
                0x40,				// 11
                0x80,				// 12
                UNCORRECTABLE,		// 13
                UNCORRECTABLE,		// 14
                UNCORRECTABLE,		// 15
        };
uint8_t hamming_calculate_parity_2416(uint8_t first, uint8_t second);
uint8_t hamming_correct_2416(uint8_t* first, uint8_t* second, uint8_t parity);
#endif //RADIO_HAMMING_H
