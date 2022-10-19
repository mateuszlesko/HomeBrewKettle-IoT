#ifndef MAIN_H
#define MAIN_H
//#pragma once
    #include <stdio.h>
    #include <stddef.h>
    #include "freertos/FreeRTOS.h"
    #include "freertos/task.h"
    #include "esp_log.h"
    #include "../../app/PINOUT/Pinout.h"

    #define SETUP_LOG_TAG "SETUP"
    #define MAIN_LOG_TAG "MAIN"
    #define LOG_LEVEL_LOCAL ESP_LOG_VERBOSE
    
    #define pdSECOND pdMS_TO_TICKS(1000)
    
    class Main final{
        public:
            Main(void);
            esp_err_t setupHardware(void);
            void ledsTest(void);
            void run(void);
    };
#endif