#include "main.h"

esp_adc_cal_characteristics_t bottom_sensor_adc1_characts;
esp_adc_cal_characteristics_t top_sensor_adc1_characts;
PinADC1 *p_sensor_bottom; // sensor at the bottom
PinADC1 *p_sensor_top; // sensor at the top

int sec_counter = 0;
static SemaphoreHandle_t s_timer_sem;

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
    ESP_LOGI(HARDWARE_SETUP_TAG, "Configuring hardware");
    
    gpio_reset_pin(HEATER_GPIO);
    gpio_set_direction(HEATER_GPIO, GPIO_MODE_OUTPUT);
    gpio_reset_pin(PUMP_GPIO);
    gpio_set_direction(PUMP_GPIO, GPIO_MODE_OUTPUT);
    
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
   
    //TIMER SETUP
    s_timer_sem = xSemaphoreCreateBinary();
    if (s_timer_sem == NULL) {
        printf("Binary semaphore can not be created");
        return;
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
    
    //WIFI SETUP
    init_wifi_connection();
      
    uint32_t bottom_sensor_measurement,top_sensor_measurement,bottom_temperature,top_temperature, average_temperature = 0;
    
    vTaskDelay(500);
    
    http_get_request_handler http_get_request_w_no_return = &client_event_HTTP_GET_none_return_data_handler;
    http_get_request_handler http_get_request_w_return = &client_event_HTTP_GET_w_return_data_handler;
    
    send_http_get_request(READY_TO_WORK_URL, http_get_request_w_no_return);
        
    send_http_get_request(GET_PROCEDURE_URL, http_get_request_w_return);
    Mashing mashing = {};
    Mashing *p_m = &mashing;
    deserialize_json_to_mashing_recipe(http_data_buffer,p_m, mashing_temperatures,mashing_temperature_holdings);
    
    RemoteControl remote_control = {};
    RemoteControl* p_rc = &remote_control;
    
    timer_start(TIMER_GROUP_0, TIMER_0);    
    while(true)
    {
      // every 30s do following tasks:
      if(sec_counter == 30)
      {
        timer_pause(TIMER_GROUP_0,TIMER_0);
        
        // check if the procedure reach its end
        if(p_m->actual_stage == p_m->num_stages)
        { 
            gpio_set_level(HEATER_GPIO,0);
            gpio_set_level(PUMP_GPIO,0);
            send_http_get_request(PROCEDURE_FINISH_URL ,http_get_request_w_no_return);
            continue;
        }
        
        //check remote control signals
        send_http_get_request(REMOTE_CONTROL_URL, http_get_request_w_return);
        deserialize_json_to_remote_control(http_data_buffer, p_rc);
        //check if procedure has to be paused
        if((p_rc->control_signals == REMOTE_PROCESS_PAUSE) || (p_rc->control_signals == REMOTE_PROCESS_FINISH))
        {
            gpio_set_level(HEATER_GPIO,0);
            gpio_set_level(PUMP_GPIO,0);
            continue;
        }  
         
        // check if the temperautre time holding equals the one in recipe
        //120 because: mashing times is in minutes and we checking it every 30s
        if(p_m->actual_time_holding == 120*mashing_temperature_holdings[p_m->actual_stage])
        {
            p_m->actual_time_holding = 0;
            p_m->actual_stage++;
        }
        
        gpio_set_level(PUMP_GPIO,1);
        
        int ref_temperature = mashing_temperatures[p_m->actual_stage];
        bottom_sensor_measurement = measure_mV_method1(p_sensor_bottom);
        top_sensor_measurement = measure_mV_method1(p_sensor_top);
        bottom_temperature = bottom_sensor_measurement / 10;
        top_temperature = top_sensor_measurement / 10;
        
        average_temperature = (bottom_temperature + top_temperature) / 2;
        
        
          
        if((average_temperature - SENSOR_RELATIVE_ERROR <= ref_temperature) && (average_temperature + SENSOR_RELATIVE_ERROR < ref_temperature))
        {
           ESP_LOGI(MEASUREMENT_TAG,"time holding: %d / %d",p_m->actual_time_holding,2*60*mashing_temperature_holdings[p_m->actual_stage]);
           gpio_set_level(HEATER_GPIO,1);
        }
          
        if(!((average_temperature - SENSOR_RELATIVE_ERROR <= ref_temperature) && (average_temperature + SENSOR_RELATIVE_ERROR < ref_temperature)))
        {
           gpio_set_level(HEATER_GPIO,0);
        }
          
        if((average_temperature - SENSOR_RELATIVE_ERROR == ref_temperature) || (average_temperature + SENSOR_RELATIVE_ERROR == ref_temperature))
        {
            p_m->actual_time_holding++;
        } 
           
        ESP_LOGI(MEASUREMENT_TAG, "BOTTOM ADC : %d mV = %d C \n TOP ADC : %d mV = %d C ", bottom_sensor_measurement,bottom_temperature,top_sensor_measurement,top_temperature);   
        sprintf(process_raport_url,MASHING_RAPORT_URL,p_m->recipe_id, p_m->actual_stage,bottom_temperature,top_temperature,0,CALC_INTER_TO_MIN(p_m->actual_time_holding));
        send_http_get_request(process_raport_url,http_get_request_w_no_return);
        sec_counter = 0;
        timer_start(TIMER_GROUP_0,TIMER_0);
      }
          vTaskDelay(10); //to feed watchdog
    }    
}    