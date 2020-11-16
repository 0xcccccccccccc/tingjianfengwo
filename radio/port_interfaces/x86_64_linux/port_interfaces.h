//
// Created by ma on 2020/11/13.
//

#ifndef RADIO_PORT_INTERFACES_H
#define RADIO_PORT_INTERFACES_H
#include <stdint.h>
#include "../../common/signal_t/signal_t.h"
#include "../../common/package_t/package_t.h"

void interface_delay_ms(float ms);
void interface_play_pcm_sync(int16_t *data, int32_t length);
SIGNAL interface_get_pcm_sync(double ms);
double interface_get_current_ms();
void interface_user_init();
void interface_station_init();
PACKAGE interface_user_get_package();
#endif //RADIO_PORT_INTERFACES_H
