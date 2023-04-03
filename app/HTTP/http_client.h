#ifndef HTTP_H

#define HTTP_H

#include <string.h>
#include "esp_netif.h"
#include "esp_http_client.h"
#include "esp_log.h"

#define HTTP_TAG "HTTP CLIENT"

#define BUFFER_SIZE 360

#define URL_SIZE 128

#define READY_TO_WORK_URL "http://192.168.137.1:9990/mashtun/xxx/ready"

#define GET_PROCEDURE_URL "http://192.168.137.1:9990/mashtun/xxx/procedure"
#define PROCEDURE_FINISH_URL "http://192.168.137.1:9990/mashtun/xxx/finish"

#define REMOTE_CONTROL_URL "http://192.168.137.1:9990/mashtun/xxx/remote-control"

#define MASHING_RAPORT_URL "http://192.168.137.1:9990/mashtun/xxx/report?sid=%d&bs=%d&ts=%d&pmp=%d&ht=%d&ps=%d" 

char http_data_buffer[BUFFER_SIZE];

char process_raport_url[URL_SIZE];

typedef esp_err_t(*http_get_request_handler)(esp_http_client_event_handle_t evt);

esp_err_t send_http_get_request(char* url, http_get_request_handler http_handler);

esp_err_t client_event_HTTP_GET_none_return_data_handler(esp_http_client_event_handle_t evt);

esp_err_t client_event_HTTP_GET_w_return_data_handler(esp_http_client_event_handle_t evt);

void reset_http_client();

#endif