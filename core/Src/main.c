#include "main.h"

//TODO: take these to heap
//unsigned char processControlSignals = 0x0;
//int currentRecipeIndex;
//int temperatureHolding;
//int i = 0;

//double temperature = 0.0;    
//
esp_adc_cal_characteristics_t p35_adc1_characts;
PinADC1 *p_adc1;
//

#define TIMER_DIVIDER   (40000)

//uint8_t process_control_signals = 0;

int sec_counter = 0;
static SemaphoreHandle_t s_timer_sem;

static bool IRAM_ATTR timer_group_isr_callback(void * args)
{
    BaseType_t high_task_awoken = pdFALSE;
    xSemaphoreGiveFromISR(s_timer_sem, &high_task_awoken);
    //processControlSignals |= (1 << 0);
    sec_counter++;
    //timer_pause(TIMER_GROUP_0,TIMER_0);
    return (high_task_awoken == pdTRUE);    
}

void app_main(void)
{ 
    ESP_LOGI(HARDWARE_SETUP_TAG, "Configuring hardware");
    
    gpio_reset_pin(GPIO_NUM_4);
    gpio_set_direction(GPIO_NUM_4, GPIO_MODE_OUTPUT);
    
    free(p_adc1);
    p_adc1 = (PinADC1*)calloc(1,sizeof(PinADC1));
    p_adc1->channel = ADC_CHANNEL_7;
    p_adc1->atten = ADC_ATTEN_DB_11;
    p_adc1->width = ADC_WIDTH_BIT_11;
    p_adc1->samples = 32;
    p_adc1->p_characts = &p35_adc1_characts;
    
    //p_adc1 = (PinADC1*) calloc(1,sizeof(PinADC1));
    //p_adc1 = &adc1;
    
    configure_adc1(p_adc1);
//    adc1_config_channel_atten(ADC1_CHANNEL_7, ADC_ATTEN_DB_11);
//    adc1_config_width(ADC_WIDTH_BIT_11);
//    esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_11, 0, &adc1_chars);
   
    s_timer_sem = xSemaphoreCreateBinary();
    if (s_timer_sem == NULL) {
        printf("Binary semaphore can not be created");
    }
    timer_config_t config = {
        .divider = TIMER_DIVIDER,
        .counter_dir = TIMER_COUNT_UP,
        .counter_en = TIMER_PAUSE,
        .alarm_en = TIMER_ALARM_EN,
        .auto_reload = TIMER_AUTORELOAD_EN
    };
    timer_init(TIMER_GROUP_0, TIMER_0, &config);
    timer_set_counter_value(TIMER_GROUP_0, TIMER_0, 0);
    
    timer_set_alarm_value(TIMER_GROUP_0, TIMER_0, (TIMER_BASE_CLK / TIMER_DIVIDER));
    timer_enable_intr(TIMER_GROUP_0, TIMER_0);
    timer_isr_callback_add(TIMER_GROUP_0, TIMER_0, timer_group_isr_callback, NULL, 0);
    timer_start(TIMER_GROUP_0, TIMER_0);

    
    //free(p_wifi);
//    p_wifi = (WiFi*)calloc(1,sizeof(WiFi));
//    p_wifi->hostname = "mashtum";
//    p_wifi->config = {
//        
//    };
//    p_wifi->macAddress = {0x70, 0xB3, 0xD5, 0x6A, 0x02, 0x01};
    
    
    //ESP_ERROR_CHECK(establish_wifi_connection());
    uint32_t mv1,tmp1 = 0;
    uint64_t timerValue = 0;
    double timerSec = 0.0;
    
    //timer_start(TIMER_GROUP_0, TIMER_0);    
    while(true)
    {
      if(sec_counter == 30)
      {
        mv1 = measure_mV_method1(p_adc1);
        tmp1 = mv1 / 10;
        ESP_LOGI(MEASUREMENT_TAG, "ADC : %d mV = %d C", mv1,tmp1);   
        sec_counter = 0;
      }
        //        raw = adc1_get_raw(ADC_CHANNEL_7);
//        mv1 = esp_adc_cal_raw_to_voltage(raw,&adc1_chars);
//        tmp1 = mv1 / 10;
//        t = ((raw / 2048) * 3.3 - 1.6173) / -0.0037;
          vTaskDelay(10); //to feed watchdog
    }    
    //fflush(stdout);
}


    