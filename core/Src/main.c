#include "main.h"

esp_adc_cal_characteristics_t bottom_sensor_adc1_characts;
esp_adc_cal_characteristics_t top_sensor_adc1_characts;
PinADC1 *p_sensor_bottom; // sensor at the bottom
PinADC1 *p_sensor_top; // sensor at the top

//uint8_t process_control_signals = 0;

int sec_counter = 0;
static SemaphoreHandle_t s_timer_sem;

//Mashing *p_mashing;
int mashing_temperatures[MAX_STAGES_NUM];
int mashing_temperature_holdings[MAX_STAGES_NUM];

int actual_stage = 0;
int actual_time_holding = 0;

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
    
    free(p_sensor_bottom);
    p_sensor_bottom = (PinADC1*)calloc(1,sizeof(PinADC1));
    p_sensor_bottom->channel = ADC_CHANNEL_7;
    p_sensor_bottom->atten = ADC_ATTEN_DB_11;
    p_sensor_bottom->width = ADC_WIDTH_BIT_11;
    p_sensor_bottom->samples = 32;
    p_sensor_bottom->p_characts = &bottom_sensor_adc1_characts;
    configure_adc1(p_sensor_bottom);
    
    free(p_sensor_top);
    p_sensor_top = (PinADC1*)calloc(1, sizeof(PinADC1));
    p_sensor_top->channel = ADC_CHANNEL_6;
    p_sensor_top->atten = ADC_ATTEN_DB_11;
    p_sensor_top->width = ADC_WIDTH_BIT_11;
    p_sensor_top->samples = 32;
    p_sensor_top->p_characts = &top_sensor_adc1_characts;
    configure_adc1(p_sensor_top);
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
    
    uint32_t bottom_sensor_measurement,top_sensor_measurement,bottom_temperature,top_temperature, average_temperature = 0;
    
    vTaskDelay(500);
    
    HTTP_GET_raport_readiness();
    
    actual_state = IDLE;
    
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
        int ref = mashing_temperatures[actual_stage];
        timer_pause(TIMER_GROUP_0,TIMER_0);
        bottom_sensor_measurement = measure_mV_method1(p_sensor_bottom);
        top_sensor_measurement = measure_mV_method1(p_sensor_top);
        bottom_temperature = bottom_sensor_measurement / 10;
        top_temperature = top_sensor_measurement / 10;
        
        average_temperature = (bottom_temperature + top_temperature) / 2;
        if(actual_stage == p_m->num_stages)
        {
            //control_signals = 0x00;
            
            continue;
        }
        if(actual_time_holding == 2*60*mashing_temperature_holdings[actual_stage])
        {
            actual_time_holding = 0;
            actual_stage++;
        }
        
        if((average_temperature - ACCURACY <= ref) && (average_temperature + ACCURACY < ref))
        {
           ESP_LOGI(MEASUREMENT_TAG,"time holding: %d / %d",actual_time_holding,2*60*mashing_temperature_holdings[actual_stage]);
           actual_time_holding++;
           gpio_set_level(GPIO_NUM_4,1);
        }
          
        if(!((average_temperature - ACCURACY <= ref) && (average_temperature + ACCURACY < ref)))
        {
           gpio_set_level(GPIO_NUM_4,0);
        }
           
        ESP_LOGI(MEASUREMENT_TAG, "BOTTOM ADC : %d mV = %d C \n TOP ADC : %d mV = %d C ", bottom_sensor_measurement,bottom_temperature,top_sensor_measurement,top_temperature);   
        HTTP_GET_mashing_raport(1,1,bottom_temperature,top_temperature,5);
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


    