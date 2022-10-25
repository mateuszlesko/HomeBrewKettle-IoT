#include "main.h"

static Main m;

PINOUT::PinOutput heater(PINOUT::HEATER_PIN);
PINOUT::PinOutput pump(PINOUT::PUMP_PIN);
PINOUT::PinOutput startLED(PINOUT::LED_START_PIN);
PINOUT::PinOutput idleLED(PINOUT::LED_IDLE_PIN);
PINOUT::PinOutput emergencyLED(PINOUT::LED_EMERGENCY_PIN);
ADC::PinADC1 sensor1;
SENSORS::LM35 lm35;

extern "C" void app_main(void)
{
    ESP_ERROR_CHECK(m.setupHardware());   
    while(true)
    {
        m.run();
    }
    fflush(stdout);
}

Main::Main(void){
}

esp_err_t Main::setupHardware(void)
{
    ESP_LOGI(SETUP_LOG_TAG,"configuring general peryferials");
    esp_err_t status{ESP_OK};
    //ADC::PinADC1 sensor1;
    sensor1.setADCChannel(ADC::SENSOR2_ADC);
    sensor1.setADCAttennuation(ADC_ATTEN_DB_11);
    //sensor1.setADCBitsWidth(ADC_WIDTH_MAX);
    sensor1.calibrateADC();
    sensor1.configureADC();
    lm35.setADC(sensor1);
    return status;  
}

void Main::ledsTest(void)
{
    for(int i = 0; i < 5; i++){
        idleLED.setPinState(PINOUT::HIGH);
        startLED.setPinState(PINOUT::HIGH);
        emergencyLED.setPinState(PINOUT::HIGH);
        vTaskDelay(2*pdSECOND);
        idleLED.setPinState(PINOUT::LOW);
        startLED.setPinState(PINOUT::LOW);
        emergencyLED.setPinState(PINOUT::LOW);
    }
    
}

void Main::run(void)
{
    uint32_t mv = sensor1.measure();
    printf("voltage: %d mV \n", mv);
    //uint32_t scaled_mv = sensor1.scale(mv,0,5000,150,2450);
    //printf("scaled voltage: %d mV \n", scaled_mv);
    double temperature = lm35.readTemperature();
    printf("temperature: %lf C \n", temperature);
   //ESP_LOGI(MAIN_LOG_TAG,"Hello there!");
    
    //m.ledsTest();
    vTaskDelay(pdSECOND);
}