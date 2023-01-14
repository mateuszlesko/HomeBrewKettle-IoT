#ifndef MAIN_H

#define MAIN_H

#include <stdio.h>
#include <stddef.h>
<<<<<<< HEAD
=======
#include <string.h>
>>>>>>> a7b3513bb6b34af888b0dd8813c3e9e4c16e4598
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "freertos/semphr.h"
#include "driver/timer.h"
<<<<<<< HEAD

#include "../../app/PINOUT/Pinout.h"
#include "../../app/ADC/PinADC1.h"
#include "../../app/SENSORS/LM35.h"

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

=======
#include "driver/gpio.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"
#include "soc/timer_group_struct.h"
#include "soc/timer_group_reg.h"
#include "../../app/STATE_MACHINE/state_machine.h"
#include "../../app/ADC/adc.h"
#include "../../app/WIFI/wifi.h"
#include "../../app/HTTP/http_client.h"
#include "../../app/MASHING_CONTROL/mashing_control.h"
#include "../../app/JSON_HELPER/json_helper.h"

#define HARDWARE_SETUP_TAG "SETUP: HARDWARE"
#define WIFI_SETUP_TAG "SETUP: WIFI"

#define DEBUG_TAG "DEBUG:"
#define MEASUREMENT_TAG "MEASUREMENT:"

#define TIMER_DIVIDER   (40000)

#define MAX_STAGES_NUM  8

static bool IRAM_ATTR timer_group_isr_callback(void * args);

static SystemState actual_state = RESET;

//static unsigned char control_signals = 0x00;

#define MISFUNCTION_BIT 4
#define IDLE_BIT 5
#define IN_PROCESS_BIT 6
#define FINISH_PROCESS_BIT 7


>>>>>>> a7b3513bb6b34af888b0dd8813c3e9e4c16e4598
#endif