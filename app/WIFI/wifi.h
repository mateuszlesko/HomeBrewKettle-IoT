#ifndef WIFI_H

#define WIFI_H

#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "nvs_flash.h"
#include "esp_wifi.h"

#define HOSTNAME "mashtum-N7"

#define SSID "R1"

#define PASSWORD "8C739y!1"

extern uint8_t mac[6];

void wifi_event_handler(void *event_handler_arg, esp_event_base_t event_base, int32_t event_id, void *event_data);

esp_err_t init_wifi_connection();

#endif