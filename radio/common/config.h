//
// Created by ma on 2020/11/10.
//

#ifndef RADIO_CONFIG_H
#define RADIO_CONFIG_H

#define FS 8000.0
#define SYNC_HEADER 0b01110010
#define CH1_FREQ 1000.0
#define CH2_FREQ 1500.0
#define CH3_FREQ 2000.0
#define CH4_FREQ 2500.0


#define TONE_LENGTH 125.0 // how many points will be in a singal tone
#define TONE_SPACING_LENGTH 0.0 // how many slient points will be between each tones

// |----|-------------|----|----|-------------|----|
// |  D |   DOWN      |  D |  D |   DOWN      |  D |
// |----|-------------|----|----|-------------|----|

// |-----------------------|-----------------------|
// |        UP             |            UP         |
// |-----------------------|-----------------------|

#define TIME_DOWN_MS ((((float)TONE_LENGTH+(float)TONE_SPACING_LENGTH)/(float)FS)*8.0*4.0*1000.0)
#define TIME_DOWN_D_MS ((float)(0.4*TIME_DOWN_MS))
#define TIME_UP_MS (2.0*TIME_DOWN_D_MS+TIME_DOWN_MS)

#endif //RADIO_CONFIG_H


