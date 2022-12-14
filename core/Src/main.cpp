#include "main.h"

static Main m;


//TODO: take these to heap
PINOUT::PinOutput heater(PINOUT::HEATER_PIN);
//PINOUT::PinOutput pump(PINOUT::PUMP_PIN);
//PINOUT::PinOutput startLED(PINOUT::LED_START_PIN);
//PINOUT::PinOutput idleLED(PINOUT::LED_IDLE_PIN);
//PINOUT::PinOutput emergencyLED(PINOUT::LED_EMERGENCY_PIN);
ADC::PinADC1 sensor1;
SENSORS::LM35 lm35;

static int a = 0;
//static SemaphoreHandle_t intervalTimerSem;
unsigned char processControlSignals = 0x0;
extern "C" 
{
    
    static bool IRAM_ATTR timer_group_isr_callback(void * args)
    {
        a++;
        
        BaseType_t high_task_awoken = pdFALSE;
        xSemaphoreGiveFromISR(intervalTimerSem, &high_task_awoken);
        processControlSignals |= (1 << 0);
        return (high_task_awoken == pdTRUE);    
    }
    
    void app_main(void)
    {
        
        ESP_ERROR_CHECK(m.setupHardware());   
        
        intervalTimerSem = xSemaphoreCreateBinary();
        timer_config_t config;
        config.divider = 972;
        config.counter_dir = TIMER_COUNT_UP;
        config.counter_en = TIMER_PAUSE;
        config.alarm_en = TIMER_ALARM_EN;
        config.auto_reload = TIMER_AUTORELOAD_EN;
        
        //init timer
        timer_init(TIMER_GROUP_0, TIMER_0, &config);
        timer_set_counter_value(TIMER_GROUP_0, TIMER_0, 0); //select timer and set init value
        /* Base clock speed of ESP32 is 80MHz, we want down it do like 5kHz. To acomplish that we need to set prescaler as a time divider*/
        timer_set_alarm_value(TIMER_GROUP_0, TIMER_0, (TIMER_BASE_CLK / TIMER_DIVIDER)); // = 80MHz / 16 MHz = 5 Hz
        timer_enable_intr(TIMER_GROUP_0, TIMER_0);
        timer_isr_callback_add(TIMER_GROUP_0, TIMER_0, timer_group_isr_callback, NULL, 0);
        timer_start(TIMER_GROUP_0, TIMER_0);
        
        while(true)
        {
            m.run();
        }
        
        fflush(stdout);
    }
}

Main::Main(void){}

esp_err_t Main::setupHardware(void)
{
    ESP_LOGI(SETUP_LOG_TAG,"configuring general peryferials");
    esp_err_t status{ESP_OK};
    //ADC::PinADC1 sensor1;
    
    sensor1.setADCChannel(ADC::SENSOR2_ADC);
    sensor1.setADCAttennuation(ADC_ATTEN_DB_11);
    sensor1.setSamples(64);
    sensor1.calibrateADC();
    sensor1.configureADC();
    lm35.setADC(sensor1);
    return status;  
}

void Main::run(void)
{
    int temperature = 0;
    double sec = 0;
    timer_get_counter_time_sec(TIMER_GROUP_0, TIMER_0,&sec);
    printf("%f sec \n",sec);
    if(processControlSignals & (1<<0))
    {
        int mv = sensor1.measure();
        temperature = lm35.readTemperature();
        printf("voltage: %d mV \n temperature: %d \n", mv,temperature);
        vTaskDelay(pdSECOND);  
        processControlSignals &= ~(1<<0);
        temperature >= 67 ? heater.setPinState(PINOUT::LOW) : heater.setPinState(PINOUT::HIGH);
    }
    vTaskDelay(pdSECOND); 
        
}
    