#include "main.h"

//TODO: take these to heap

//static SemaphoreHandle_t intervalTimerSem;
unsigned char processControlSignals = 0x0;
int currentRecipeIndex;
int temperatureHolding;
int i = 0;

double temperature = 0.0;    

static bool IRAM_ATTR timer_group_isr_callback(void * args)
{
    BaseType_t high_task_awoken = pdFALSE;
    xSemaphoreGiveFromISR(intervalTimerSem, &high_task_awoken);
    processControlSignals |= (1 << 0);
    i++;
    return (high_task_awoken == pdTRUE);    
}

void configureHardware(void)
{
    ESP_LOGI(HARDWARE_SETUP_TAG, "Configuring hardware");
    gpio_reset_pin(GPIO_NUM_4);
    gpio_set_direction(GPIO_NUM_4, GPIO_MODE_OUTPUT);
    
    adc1_config_channel_atten(ADC1_CHANNEL_7, ADC_ATTEN_DB_11);
    adc1_config_width(ADC_WIDTH_BIT_11);
    esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_11, 0, &adc1_chars);
   
//    intervalTimerSem = xSemaphoreCreateBinary();
//    timer_config_t config;
//    config.divider = 8000;//972; //for 10s
//    config.counter_dir = TIMER_COUNT_UP;
//    config.counter_en = TIMER_PAUSE;
//    config.alarm_en = TIMER_ALARM_EN;
//    config.auto_reload = TIMER_AUTORELOAD_EN;
//        //init timer
//    timer_init(TIMER_GROUP_0, TIMER_0, &config);
//    timer_set_counter_value(TIMER_GROUP_0, TIMER_0, 0); //select timer and set init value
//        /* Base clock speed of ESP32 is 80MHz, we want down it do like 5kHz. To acomplish that we need to set prescaler as a time divider*/
//    timer_set_alarm_value(TIMER_GROUP_0, TIMER_0, 60000);
//    timer_enable_intr(TIMER_GROUP_0, TIMER_0);
//    timer_isr_callback_add(TIMER_GROUP_0, TIMER_0, timer_group_isr_callback, NULL, 0);
    
}


void app_main(void)
{ 
    configureHardware();
    uint32_t raw,mv1,tmp1 = 0;
    double t = 0.0;
    timer_start(TIMER_GROUP_0, TIMER_0);
    while(true)
    {
        raw = adc1_get_raw(ADC_CHANNEL_7);
        mv1 = esp_adc_cal_raw_to_voltage(raw,&adc1_chars);
        tmp1 = mv1 / 10;
        t = ((raw / 2048) * 3.3 - 1.6173) / -0.0037;
        ESP_LOGI(MEASUREMENT_TAG, "ADC raw: %d = %d mV = %d C |%lf C", raw,mv1,tmp1,t);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }    
    fflush(stdout);
}


    