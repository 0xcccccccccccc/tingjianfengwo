//
// Created by ma on 2020/11/10.
//

#ifndef RADIO_RADIO_TX_H
#define RADIO_RADIO_TX_H

#include <stdint.h>
#include "../common/signal_t/signal_t.h"
#include "../common/bit_stream_t/bit_stream_t.h"

#define PI 3.14159265358979323825

SIGNAL radio_tx_generate_signal_from_byte(double target_freq, uint8_t raw_data);
SIGNAL radio_tx_generate_signal_from_bytes(double target_freq, uint8_t* raw_data,uint32_t data_length);
BIT_STREAM radio_tx_generate_sync_header();
#endif //RADIO_RADIO_TX_H
