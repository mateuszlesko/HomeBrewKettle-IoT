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

#define MAX_STAGES_NUM  10

static bool IRAM_ATTR timer_group_isr_callback(void * args);

//static unsigned char control_signals = 0x00;

#define MISFUNCTION_BIT 4
#define IDLE_BIT 5
#define IN_PROCESS_BIT 6
#define FINISH_PROCESS_BIT 7


#endif