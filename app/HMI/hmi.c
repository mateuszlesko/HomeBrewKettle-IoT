#include "hmi.h"

#define WIDTH_128 (128)
#define HEIGHT_64 (64)

#define MAX_LINE_CHARS (20)

char lineChar[MAX_LINE_CHARS];

#define DEACTIVETED_LINE_1 "ready to work"
#define DEACTIVETED_LINE_2 "please send recipe"

#define PAUSED_LINE_1 "R: #%d"
#define PAUSED_LINE_2 "S : %d / %d"
#define PAUSED_LINE_3 "= PAUSED ="

#define PROCESS_LINE_1 "R: #%d"
#define PROCESS_LINE_2 "S : %d / %d"
#define PROCESS_LINE_3 "AVG T: %d C"
#define PROCESS_LINE_4 "REF T: %d C"
#define PROCESS_LINE_5 "PROG: %d %"
#define PROCESS_LINE_6 "= WORK ="

static SSD1306_t dev;

#if CONFIG_SSD1306_128x64
T_HMI_SCREEN* HMI_init_screen_128x64(unsigned int center, unsigned int top, unsigned int bottom)
{
    ESP_LOGI(HMI_CONFIG_TAG, "HMI INIT \n");
    T_HMI_SCREEN* hmi = (T_HMI_SCREEN*) calloc(1,sizeof(T_HMI_SCREEN));
    hmi->center = center;
    hmi->top = top;
    hmi->bottom = bottom;
    ssd1306_init(&dev, WIDTH_128, HEIGHT_64);
    ssd1306_contrast(&dev, 0xff);
    hmi->p_dev = &dev;
    return hmi;
}
#endif // CONFIG_SSD1306_128x64

#if CONFIG_I2C_INTERFACE
void HMI_i2c_init(T_HMI_SCREEN* p_hmi)
{
    ESP_LOGI(HMI_CONFIG_TAG, "INTERFACE is i2c \n");
    i2c_master_init(p_hmi->p_dev, CONFIG_SDA_GPIO, CONFIG_SCL_GPIO, CONFIG_RESET_GPIO);
}
#endif // CONFIG_I2C_INTERFACE

void HMI_clean_screen(T_HMI_SCREEN* p_hmi)
{
    ssd1306_clear_screen(p_hmi->p_dev, false);
}

void HMI_Mashtum_is_READY(T_HMI_SCREEN* p_hmi)
{
    ssd1306_clear_screen(p_hmi->p_dev, false);
    char msg1[] = "ready to work"; 
    ssd1306_display_text(p_hmi->p_dev, 0, msg1, (sizeof(msg1) / 8)+1, false);
}

void HMI_Mashtum_is_PAUSED(T_HMI_SCREEN* p_hmi, T_MashingProcess* p_mashing)
{
    ssd1306_clear_screen(p_hmi->p_dev, false);
    char line1[12];
    char line2[12];

    sprintf(line1,PAUSED_LINE_1, p_mashing->recipe_id);
    sprintf(line2,PAUSED_LINE_2, p_mashing->actual_stage, p_mashing->num_stages);
    ssd1306_display_text(p_hmi->p_dev, 1, line1, sizeof(line1) / 8, false);
    ssd1306_display_text(p_hmi->p_dev, 2, line1, sizeof(line2) / 8, false);
    ssd1306_display_text(p_hmi->p_dev, 3, PAUSED_LINE_3, sizeof(PAUSED_LINE_3) / 8, false);
}

//void HMI_Mashtum_is_WORKING(T_HMI_SCREEN* p_hmi, unsigned int recipe_id, unsigned int curr_stage, unsigned int all_stages, int avg_t, int ref_t, unsigned int prog_percs)
//{
//    ssd1306_clear_screen(p_hmi->p_dev, false);
//    char line1[12];
//    char line2[12];
//    sprintf(line1,PROCESS_LINE_1, recipe_id);
//    sprintf(line2,PROCESS_LINE_2, curr_stage, all_stages);
//    ssd1306_display_text(p_hmi->p_dev, 1, line1, CALC_STR_LEN(line1), false);
//    ssd1306_display_text(p_hmi->p_dev, 2, line1, CALC_STR_LEN(line2), false);
//}