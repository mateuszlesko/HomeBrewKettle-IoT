#ifndef MAIN_H
#define MAIN_H
#pragma once
    #include <stdio.h>
    #include <stddef.h>
    #include "freertos/FreeRTOS.h"
    #include "freertos/task.h"
    #include "esp_log.h"
    #include "../lib/PINOUT/gpio.h"

    #define SETUP_LOG_TAG "SETUP"
    #define MAIN_LOG_TAG "MAIN"
    #define LOG_LEVEL_LOCAL ESP_LOG_VERBOSE
    
    #define pdSECOND pdMS_TO_TICKS(1000)
    
    esp_err_t setup_gpio(void);
    
    class Main final{
        public:
            Main(void);
            esp_err_t setup_hardware(void);
            void run(void);
    };
#endif