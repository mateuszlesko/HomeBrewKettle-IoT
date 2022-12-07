#ifndef MAIN_H

#define MAIN_H

#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "freertos/semphr.h"
#include "driver/timer.h"
#include "driver/gpio.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"
#include "../../app/STATE_MACHINE/state_machine.h"
#include "../../app/ADC/adc.h"


#define HARDWARE_SETUP_TAG "SETUP: HARDWARE"
#define WIFI_SETUP_TAG "SETUP: WIFI"

#define DEBUG_TAG "DEBUG:"
#define MEASUREMENT_TAG "MEASUREMENT:"

static bool IRAM_ATTR timer_group_isr_callback(void * args);

static SemaphoreHandle_t intervalTimerSem;

static SystemState actual_state = RESET;

void configureHardware(void);

#endif