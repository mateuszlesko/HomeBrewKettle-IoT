#ifndef MAIN_H

#define MAIN_H

#include <stdio.h>
#include <stddef.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "freertos/semphr.h"
#include "driver/timer.h"
#include "driver/gpio.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"
#include "../../app/STATE_MACHINE/StateMachine.h"


#define HARDWARE_SETUP_TAG "SETUP: HARDWARE"
#define WIFI_SETUP_TAG "SETUP: WIFI"


#define MEASUREMENT_TAG "MEASUREMENT:"

#define TIMER_DIVIDER   (16)
#define pdSECOND pdMS_TO_TICKS(1000)
#define CYCLES_TO_MEASUREMENT 12000

static bool IRAM_ATTR timer_group_isr_callback(void * args);

static SemaphoreHandle_t intervalTimerSem;
esp_adc_cal_characteristics_t adc1_chars;

state actual_state = RESET;
//actuator *p_heater, *p_pump;

void configureHardware(void);

#endif