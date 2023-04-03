#ifndef HMI_H

#define HMI_H

#include "esp_log.h"
#include "../MASHING_CONTROL/mashing_control.h"
#include "../SSD1306/font8x8_basic.h"
#include "../SSD1306/ssd1306.h"

#define HMI_LINES (8)
#define HMI_LINE_CHARS (20)

#define HMI_MSG_COUNTER (3)

#define HMI_CONFIG_TAG ("HMI CONFIG")

typedef struct
{
    unsigned int center;
    unsigned int top;
    unsigned int bottom;
    SSD1306_t* p_dev;
} T_HMI_SCREEN;


T_HMI_SCREEN* HMI_init_screen_128x64(unsigned int center, unsigned int top, unsigned int bottom);


void HMI_i2c_init(T_HMI_SCREEN* p_hmi);

void write_init_screen(T_HMI_SCREEN* hmi);

void write_paused_screen(T_HMI_SCREEN* hmi);

void HMI_clean_screen(T_HMI_SCREEN* p_hmi);

void HMI_Mashtum_is_READY(T_HMI_SCREEN* p_hmi);
void HMI_Mashtum_is_PAUSED(T_HMI_SCREEN* p_hmi, T_MashingProcess* p_mashing);
//void HMI_Mashtum_is_WORKING(T_HMI_SCREEN* p_hmi, T_MashingProcess* p_mashing);
#endif