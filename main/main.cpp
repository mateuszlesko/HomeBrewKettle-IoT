#include "main.h"
#include "lib/GPIO/gpio.h"

static Main m;

extern "C" void app_main(void)
{
    ESP_ERROR_CHECK(m.setup());
    while(true)
    {
        m.run();
    }
    fflush(stdout);
}

Main::Main(void){}

esp_err_t Main::setup(void)
{
    ESP_LOGI(SETUP_LOG_TAG,"configuring general peryferials");
    gpio_reset_pin(GPIO:HEATER_PIN);
    /* Set the GPIO as a push/pull output */
    gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);
    esp_err_t status{ESP_OK};
    return status;  
}

void Main::run(void)
{
    ESP_LOGI(MAIN_LOG_TAG,"Hello there!");
    gpio_set_level(GPIO:HEATER_PIN, GPIO::level.HIGH);
    vTaskDelay(pdSECOND);
    gpio_set_level(GPIO:HEATER_PIN, GPIO::level.LOW);
    vTaskDelay(pdSECOND);
}