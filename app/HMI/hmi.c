#include "hmi.h"

#define WIDTH_128 (128)
#define HEIGHT_64 (64)

#define LINE_LEN(x) ((x / 8) + 1)
#define MAX_LINE_CHARS (20)
char lineChar[MAX_LINE_CHARS];

#define DEACTIVETED_LINE_1 "ready to work"
#define DEACTIVETED_LINE_2 "please send recipe"

#define PAUSED_LINE_1 "S : %d / %d"
#define PAUSED_LINE_2 "= PAUSED ="

#define PROCESS_LINE_1 "S : %d / %d"
#define PROCESS_LINE_2 "AVG T: %d C"
#define PROCESS_LINE_3 "REF T: %d C"
#define PROCESS_LINE_4 "PROG: %d %"
#define PROCESS_LINE_5 "= WORK ="

static SSD1306_t dev;

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

void HMI_i2c_init(T_HMI_SCREEN* p_hmi)
{
    ESP_LOGI(HMI_CONFIG_TAG, "INTERFACE is i2c \n");
    i2c_master_init(p_hmi->p_dev, CONFIG_SDA_GPIO, CONFIG_SCL_GPIO, CONFIG_RESET_GPIO);
}

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

    sprintf(line1,PAUSED_LINE_1, p_mashing->actual_stage, p_mashing->num_stages);
    ssd1306_display_text(p_hmi->p_dev, 1, line1, LINE_LEN(sizeof(line1)), false);
    ssd1306_display_text(p_hmi->p_dev, 2, PAUSED_LINE_2,LINE_LEN(sizeof(PAUSED_LINE_2)), false);
}
