#include "main.h"


esp_adc_cal_characteristics_t p35_adc1_characts;
PinADC1 *p_adc1;

//uint8_t process_control_signals = 0;

int sec_counter = 0;
static SemaphoreHandle_t s_timer_sem;

//Mashing *p_mashing;
int mashing_temperatures[MAX_STAGES_NUM];
int mashing_temperature_holdings[MAX_STAGES_NUM];

static bool IRAM_ATTR timer_group_isr_callback(void * args)
{
    BaseType_t high_task_awoken = pdFALSE;
    xSemaphoreGiveFromISR(s_timer_sem, &high_task_awoken);
    sec_counter++;
    return (high_task_awoken == pdTRUE);    
}

void app_main(void)
{ 
    actual_state = SETUP;
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
     
    configure_adc1(p_adc1);
//    adc1_config_channel_atten(ADC1_CHANNEL_7, ADC_ATTEN_DB_11);
//    adc1_config_width(ADC_WIDTH_BIT_11);
//    esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_11, 0, &adc1_chars);
   
    //TIMER SETUP
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

    actual_state = OFFLINE;
    //WIFI SETUP
    init_wifi_connection();
      
    actual_state = ONLINE;
    
    uint32_t mv1,tmp1 = 0;
    
    vTaskDelay(500);
    
    HTTP_GET_raport_readiness();
    
    actual_state = IDLE;
    
//    if(p_mashing != NULL)
//    {
//        free(p_mashing);
//        p_mashing = NULL;
//    }
//    if(mashing_temperatures != NULL)
//    {
//        free(mashing_temperatures);
//        mashing_temperatures = NULL;
//    }
//    if(mashing_temperature_holdings != NULL)
//    {
//        free(mashing_temperature_holdings);
//        mashing_temperature_holdings = NULL;
//    }
//    if(http_data_buffer != NULL)
//    {
//        free(http_data_buffer);
//        http_data_buffer = NULL;
//    }
    HTTP_GET_mashing_procedure();
    ESP_LOGI(DEBUG_TAG,"%s",http_data_buffer);
    //p_mashing = (Mashing*) malloc(sizeof(Mashing));
    Mashing mashing = {};
    Mashing *p_m = &mashing;
    deserialize_json_to_mashing_recipe(http_data_buffer,p_m, mashing_temperatures,mashing_temperature_holdings);
    //deserialize_json_to_mashing_recipe("{'MID':1,'RID':1,'PC':2,'MTpL':[50,55],'MTmL':[15,12]}",p_mashing, mashing_temperatures,mashing_temperature_holdings);
    //int iterator = 0;
    int count = p_m->num_stages;
    for(int iterator=0; iterator < count; iterator++)
    {
        ESP_LOGI(DEBUG_TAG,"%d = %d C \n",iterator,mashing_temperatures[iterator]);
    }
    
    timer_start(TIMER_GROUP_0, TIMER_0);    
    while(true)
    {
      if(sec_counter == 30)
      {
        timer_pause(TIMER_GROUP_0,TIMER_0);
        mv1 = measure_mV_method1(p_adc1);
        tmp1 = mv1 / 10;
        ESP_LOGI(MEASUREMENT_TAG, "ADC : %d mV = %d C", mv1,tmp1);   
        HTTP_GET_mashing_raport(1,1,tmp1,tmp1,5);
        sec_counter = 0;
        timer_start(TIMER_GROUP_0,TIMER_0);
      }
        //        raw = adc1_get_raw(ADC_CHANNEL_7);
//        mv1 = esp_adc_cal_raw_to_voltage(raw,&adc1_chars);
//        tmp1 = mv1 / 10;
//        t = ((raw / 2048) * 3.3 - 1.6173) / -0.0037;
          vTaskDelay(10); //to feed watchdog
    }    
    //fflush(stdout);
}


    