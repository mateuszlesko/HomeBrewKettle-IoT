#include "main.h"

static Main m;

extern "C" void app_main(void)
{
    ESP_ERROR_CHECK(m.setup_hardware());
    while(true)
    {
        m.run();
    }
    fflush(stdout);
}


esp_err_t setup_gpio(void)
{
    //ESP_LOGI(PINOUT::PIN_SETUP_LOG_TAG,"configuring GPIO pins");
    PINOUT::PinOutput heater = PINOUT::PinOutput(PINOUT::HEATER_PIN);
    esp_err_t status{ESP_OK};
    return status;
}

Main::Main(void){}

esp_err_t Main::setup_hardware(void)
{
    ESP_LOGI(SETUP_LOG_TAG,"configuring general peryferials");
    setup_gpio();
    esp_err_t status{ESP_OK};
    return status;  
}

void Main::run(void)
{
    ESP_LOGI(MAIN_LOG_TAG,"Hello there!");
    vTaskDelay(pdSECOND);
}