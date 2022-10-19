#include "main.h"
//#include "../../app/PINOUT/gpio.h"

static Main m;

PINOUT::PinOutput heater(PINOUT::HEATER_PIN);
PINOUT::PinOutput pump(PINOUT::PUMP_PIN);
PINOUT::PinOutput startLED(PINOUT::LED_START_PIN);
PINOUT::PinOutput idleLED(PINOUT::LED_IDLE_PIN);
PINOUT::PinOutput emergencyLED(PINOUT::LED_EMERGENCY_PIN);


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
    
    ESP_LOGI(MAIN_LOG_TAG,"Hello there!");
    m.ledsTest();
    vTaskDelay(pdSECOND);
}