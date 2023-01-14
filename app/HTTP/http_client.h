#ifndef HTTP_H

#define HTTP_H

#include <string.h>
#include "esp_netif.h"
#include "esp_http_client.h"
#include "esp_log.h"

#define HTTP_TAG "HTTP CLIENT"

#define BUFFER_SIZE 256

#define URL_SIZE 128

#define READY_TO_WORK_URL "http://192.168.137.1:9990/mashtum/ready/"

#define GET_PROCEDURE_URL "http://192.168.137.1:9990/mashing/procedure"
#define PROCEDURE_FINISH_URL "http://192.168.137.1:9990/mashing/finish/"

#define REMOTE_CONTROL_URL "http://192.168.137.1:9990/mashing/remotecontrol/"

#define MASHING_RAPORT_URL "http://192.168.137.1:9990/mashing/raport/?rid=%d&sid=%d&bs=%d&ts=%d&cs=%d" 

char http_data_buffer[BUFFER_SIZE];

char process_raport_url[URL_SIZE];

static int http_buffer_len;

typedef esp_err_t(*http_get_request_handler)(esp_http_client_event_handle_t evt);

void send_http_get_request(char* url, http_get_request_handler http_handler);

esp_err_t client_event_HTTP_GET_none_return_data_handler(esp_http_client_event_handle_t evt);

esp_err_t client_event_HTTP_GET_w_return_data_handler(esp_http_client_event_handle_t evt);

#endif