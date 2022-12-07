#include "main.h"

//TODO: take these to heap

//static SemaphoreHandle_t intervalTimerSem;
unsigned char processControlSignals = 0x0;
int currentRecipeIndex;
int temperatureHolding;
int i = 0;

double temperature = 0.0;    

esp_adc_cal_characteristics_t p35_adc1_characts;
PinADC1 *p_adc1;

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
    
//    PinADC1 adc1 = {
//        .channel = ADC_CHANNEL_7,
//        .atten = ADC_ATTEN_DB_11,
//        .width = ADC_WIDTH_BIT_11,
//        .samples = 32,
//        .p_characts = &p35_adc1_characts
//    };
    free(p_adc1);
    p_adc1 = (PinADC1*)calloc(1,sizeof(PinADC1));
    p_adc1->channel = ADC_CHANNEL_7;
    p_adc1->atten = ADC_ATTEN_DB_11;
    p_adc1->width = ADC_WIDTH_BIT_11;
    p_adc1->samples = 32;
    p_adc1->p_characts = &p35_adc1_characts;
    //memcpy(p_adc1,&adc1,sizeof(PinADC1));
//    p_adc1 = (PinADC1*) calloc(1,sizeof(PinADC1));
//    p_adc1 = &adc1;
    
    configure_adc1(p_adc1);
//    adc1_config_channel_atten(ADC1_CHANNEL_7, ADC_ATTEN_DB_11);
//    adc1_config_width(ADC_WIDTH_BIT_11);
//    esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_11, 0, &adc1_chars);
   
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
    uint32_t mv1,tmp1 = 0;
    //timer_start(TIMER_GROUP_0, TIMER_0);
    while(true)
    {
//        raw = adc1_get_raw(ADC_CHANNEL_7);
//        mv1 = esp_adc_cal_raw_to_voltage(raw,&adc1_chars);
//        tmp1 = mv1 / 10;
//        t = ((raw / 2048) * 3.3 - 1.6173) / -0.0037;
        mv1 = measure_mV_method1(p_adc1);
        tmp1 = mv1 / 10;
        ESP_LOGI(DEBUG_TAG,"samples: %d",p_adc1->samples);
        ESP_LOGI(MEASUREMENT_TAG, "ADC : %d mV = %d C", mv1,tmp1);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }    
    fflush(stdout);
}


    