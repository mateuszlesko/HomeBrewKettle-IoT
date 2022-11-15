#ifndef MAIN_H

#define MAIN_H

#include <stdio.h>
#include <stddef.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "freertos/semphr.h"
#include "driver/timer.h"

#include "../../app/PINOUT/Pinout.h"
#include "../../app/ADC/PinADC1.h"
#include "../../app/SENSORS/LM35.h"
#include "../../app/PROCESS_CONTROL/RecipeStage.h"

#define SETUP_LOG_TAG "SETUP"
#define MAIN_LOG_TAG "MAIN"
#define LOG_LEVEL_LOCAL ESP_LOG_VERBOSE

#define MAX_RECEPTURE_STAGES 10

#define TIMER_DIVIDER   (16)
#define pdSECOND pdMS_TO_TICKS(1000)
#define CYCLES_TO_MEASUREMENT 12000


extern "C"
{
    static bool IRAM_ATTR timer_group_isr_callback(void * args);
}

static SemaphoreHandle_t intervalTimerSem;

static int cyclesCounter = 0;

class Main final{
    public:
        Main(void);
        esp_err_t setupHardware(void);
        //esp_err_t setupTimer(void);
        void ledsTest(void);
        void run(void);
};

#endif