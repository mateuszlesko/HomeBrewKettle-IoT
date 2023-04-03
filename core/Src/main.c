#include "main.h"

#define SERIAL_LOG "SL"

#define CONTROL_BTN_PIN 14

esp_adc_cal_characteristics_t bottom_sensor_adc1_characts;
esp_adc_cal_characteristics_t top_sensor_adc1_characts;
PinADC1 sensor_bottom ; // sensor at the bottom

//static volatile unsigned int Process_Blockade = 0;

//T_HMI_SCREEN* p_hmi_screen;

int sec_counter = 0;
static SemaphoreHandle_t s_timer_sem;

int mashing_temperatures[MAX_STAGES_NUM];
int mashing_temperature_holdings[MAX_STAGES_NUM];

static inline void clear_run()
{
    sec_counter = 0;
    timer_start(TIMER_GROUP_0,TIMER_0);
}

static bool IRAM_ATTR timer_group_isr_callback(void * args)
{
    BaseType_t high_task_awoken = pdFALSE;
    xSemaphoreGiveFromISR(s_timer_sem, &high_task_awoken);
    sec_counter++;
    return (high_task_awoken == pdTRUE);    
}

/******************************************************************************************** 
@func: mesh process paused / resumption by interrupt
@descr: on falling edge at CONTROL_BTN_PIN static variable Process_Blockade change its value
********************************************************************************************/
//static void control_button_isr_handler(void* args)
//{
//    Process_Blockade ^= 1;
//}

/******************************************************************************************** 
@func:  configure_gpio
@descr: configuring gpio and hardware external interrupt
********************************************************************************************/
static void configure_gpio()
{
    ESP_LOGI(HARDWARE_SETUP_TAG, "Configuring hardware");
    
    gpio_reset_pin(HEATER_GPIO);
    gpio_set_direction(HEATER_GPIO, GPIO_MODE_OUTPUT);
    gpio_set_level(HEATER_GPIO,HEATER_PASSIVE_STATE);
    
    gpio_reset_pin(PUMP_GPIO);
    gpio_set_direction(PUMP_GPIO, GPIO_MODE_OUTPUT);
    gpio_set_level(PUMP_GPIO, PUMP_PASSIVE_STATE);
    
    /* CONTROL BUTTON */
    /* CONFIGURE PIN AND INTERRUPT*/
    
//    gpio_reset_pin(CONTROL_BTN_PIN);
//    gpio_config_t pGPIOConfig;
//    pGPIOConfig.pin_bit_mask = (1ULL << CONTROL_BTN_PIN);
//    pGPIOConfig.mode = GPIO_MODE_DEF_INPUT;
//    pGPIOConfig.pull_up_en = GPIO_PULLUP_DISABLE;
//    pGPIOConfig.pull_down_en = GPIO_PULLDOWN_DISABLE;
//    pGPIOConfig.intr_type = GPIO_INTR_NEGEDGE; /* intr on falling edge */
//    
//    gpio_config(&pGPIOConfig);
//    gpio_install_isr_service(0);
//    gpio_isr_handler_add(CONTROL_BTN_PIN,control_button_isr_handler, NULL);
}

void app_main(void)
{  
    configure_gpio();
    
    sensor_bottom.channel = ADC_CHANNEL_7;
    sensor_bottom.atten = ADC_ATTEN_DB_11;
    sensor_bottom.width = ADC_WIDTH_BIT_12;
    sensor_bottom.samples = 8;
    sensor_bottom.p_characts = &bottom_sensor_adc1_characts;
    configure_adc1(&sensor_bottom);
        
    /******************************************************************************************** 
    @func: human machine interface - configuration via i2c
    @descr: conifugre oled display and show status message
    ********************************************************************************************/
//    free(p_hmi_screen);
//    p_hmi_screen = HMI_init_screen_128x64(2, 3, 8);
//    HMI_i2c_init(p_hmi_screen);
//    HMI_Mashtum_is_READY(p_hmi_screen);
    
    /******************************************************************************************** 
    @func: time counting
    @descr: if time keeping temperature is equal to the one in recipe, them actual stage+1
            and reset time holding 
    ********************************************************************************************/
    
    s_timer_sem = xSemaphoreCreateBinary();
    if (s_timer_sem == NULL) {
         ESP_LOGI(SEMAPHORE_ERR_TAG,"Binary semaphore can not be created");
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
    
    /******************************************************************************************** 
    @func: remote control and raporting to remote func
    @descr: configure connection to the local WiFi router
    ********************************************************************************************/
    init_wifi_connection();  
    vTaskDelay(500);
    
    /******************************************************************************************** 
    @func: http - remote control and raporting to remote
    @descr: create http client to remote communication
    ********************************************************************************************/
    
    http_get_request_handler http_get_request_w_no_return = &client_event_HTTP_GET_none_return_data_handler;

    http_get_request_handler http_get_request_w_return = &client_event_HTTP_GET_w_return_data_handler;
    
    /******************************************************************************************** 
    @func: mash recepture from remote
    @descr: get mashing recepture from remote server
    ********************************************************************************************/
    
    send_http_get_request(READY_TO_WORK_URL, http_get_request_w_no_return);
    send_http_get_request(GET_PROCEDURE_URL, http_get_request_w_return);    
    T_MashingProcess mashing = {};
    T_MashingProcess *p_m = &mashing;
    deserialize_json_to_mashing_recipe(http_data_buffer,p_m, mashing_temperatures,mashing_temperature_holdings);
    
    T_RemoteControl remote_control = {};
    T_RemoteControl* p_rc = &remote_control;
    
    T_Actuator actuator = {0,0};
    
    uint32_t bottom_sensor_measurement =0;
    uint32_t average_temperature = 0;
    uint32_t ref_temperature = 0;
    uint32_t bottom_bound = 0;
    uint32_t top_bound = 0;

    ESP_LOGI(MEASUREMENT_TAG, "| S_ID | Ref_Temp | Sensor_T | Ref_Time | Act_Time | H_ON | \n");
    timer_start(TIMER_GROUP_0, TIMER_0);    
    while(true)
    {
      /******************************************************************************************** 
      @descr: control mashing process every time defined by const period
      ********************************************************************************************/
      if(sec_counter == PERIODS)
      {
        timer_pause(TIMER_GROUP_0,TIMER_0);
        
        /* check if the procedure is fished if so send information */
        if(mashing_finish == MASHING_IS_FINISHED)
        {
            ESP_LOGI(DEBUG_TAG,"PROCESS FINSIHED \n");
            gpio_set_level(HEATER_GPIO,HEATER_PASSIVE_STATE);
            gpio_set_level(PUMP_GPIO,PUMP_PASSIVE_STATE);
            
            actuator.heater = 0;
            actuator.pump = 0;
    
            send_http_get_request(PROCEDURE_FINISH_URL ,http_get_request_w_no_return);
            //clear_run();
            continue;
        }
            
        /******************************************************************************************** 
        @func: remote control
        @descr: get remote control commands and obey 
        ********************************************************************************************/
        
        send_http_get_request(REMOTE_CONTROL_URL, http_get_request_w_return);
        deserialize_json_to_remote_control(http_data_buffer, p_rc);
       
        //if((Process_Blockade == 1) || (p_rc->control_signals == REMOTE_PROCESS_PAUSE) || (p_rc->control_signals == REMOTE_PROCESS_FINISH))
        if((p_rc->control_signals == REMOTE_PROCESS_PAUSE) || (p_rc->control_signals == REMOTE_PROCESS_FINISH))
        {
            actuator.heater = 0;
            actuator.pump = p_rc->pump_work == 0 ? 0 : 1;
            ESP_LOGI(DEBUG_TAG,"PROCESS STAGNED \n");
            gpio_set_level(HEATER_GPIO, HEATER_PASSIVE_STATE);
            gpio_set_level(PUMP_GPIO, p_rc->pump_work == 0 ? PUMP_PASSIVE_STATE : PUMP_ACTIVE_STATE);
        
            clear_run();
            continue;
        }  
             
        /******************************************************************************************** 
        @func: time temperature keeping monitor
        @descr: if time keeping temperature is equal to the one in recipe, them actual stage+1
                and reset time holding
        ************************* *******************************************************************/
        
        /* multiply by 60 because mashing times is in minutes, divide by period because we check by period */
        if(p_m->actual_time_holding == MIN_TO_PERIODS(mashing_temperature_holdings[p_m->actual_stage]))
        {
            p_m->actual_time_holding = 0;
            p_m->actual_stage++;
        }
        
        /* check if procedure reached its end */
        if(p_m->actual_stage == p_m->num_stages)
        { 
            mashing_finish = MASHING_IS_FINISHED;
            clear_run();
            continue;
        }
    
        /******************************************************************************************** 
        @func: temperature monitor & control actuators
        @descr: if the measured temperature is equal or less then the one from actual stage recipe
                turn on heater, if no turn off heater
        ********************************************************************************************/
          
        ref_temperature = mashing_temperatures[p_m->actual_stage];
        
        top_bound = ref_temperature + SENSOR_AVG_ABSOLUTE_ERROR;
        bottom_bound = ref_temperature - SENSOR_AVG_ABSOLUTE_ERROR;
        
//        for(int i = 0; i < sensor_bottom.samples; i++)
//        {
//            bottom_sensor_measurement += measure_mV_method1(&sensor_bottom);
//            vTaskDelay(100);
//        }
        
//        bottom_sensor_measurement /= sensor_bottom.samples;
        bottom_sensor_measurement = measure_mV_method1(&sensor_bottom);
        average_temperature = mV_TO_C(bottom_sensor_measurement);
          
        if(average_temperature < bottom_bound)
        {
           ESP_LOGI(DEBUG_TAG,"HEATER IS ON \n");
           gpio_set_level(HEATER_GPIO,HEATER_ACTIVE_STATE);
           actuator.heater = 1;
        }  
        if((average_temperature >= bottom_bound) && (average_temperature <= top_bound))
        {
           p_m->actual_time_holding++;
        }
        if(average_temperature >= bottom_bound)
        {
           ESP_LOGI(DEBUG_TAG,"HEATER IS OFF \n");
           gpio_set_level(HEATER_GPIO,HEATER_PASSIVE_STATE);
           actuator.heater = 0;
        } 
        
        /******************************************************************************************** 
        @func: mashing process remote report
        @descr: send to http server data about the process progress and its parameters
        ********************************************************************************************/
          
        ESP_LOGI(MEASUREMENT_TAG, "| %d    | %d       | %d     | %d     | %d       | %d   | \n", 
            p_m->actual_stage,
            ref_temperature,
            average_temperature,
            MIN_TO_PERIODS(mashing_temperature_holdings[p_m->actual_stage]),
            p_m->actual_time_holding,
            actuator.heater);
          
        //"http://192.168.137.1:9990/mashtun/xxx/report?sid=%d&ts=%d&bs=%d&pmp=%d&ht=%d&ps=%d" 
        sprintf(process_raport_url, MASHING_RAPORT_URL,
                p_m->actual_stage, 
                average_temperature, 
                average_temperature, 
                actuator.pump, 
                actuator.heater, 
                p_rc->control_signals);
                 
        send_http_get_request(process_raport_url,http_get_request_w_no_return);
       
        clear_run();
    }
    vTaskDelay(10); /* to feed watchdog */   
  }  
} 