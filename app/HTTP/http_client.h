#ifndef HTTP_H

#define HTTP_H

#include <string.h>
#include "esp_netif.h"
#include "esp_http_client.h"
#include "esp_log.h"

#define REMOTE_HOST "192.168.0.1"

#define HTTP_TAG "HTTP CLIENT"

#define BUFFER_SIZE 256

char http_data_buffer[BUFFER_SIZE];
  
static int http_buffer_len;

esp_err_t client_event_HTTP_GET_raport_readiness_handler(esp_http_client_event_handle_t evt);

static void HTTP_GET_raport_readiness()
{
    esp_http_client_config_t config_get = {
        .url = "http://192.168.137.1:9990/mashtum/ready/",
        .method = HTTP_METHOD_GET,
        .cert_pem = NULL,
        .event_handler = client_event_HTTP_GET_raport_readiness_handler
    };
        
    esp_http_client_handle_t client = esp_http_client_init(&config_get);
    esp_http_client_perform(client);
    esp_http_client_cleanup(client);
}

esp_err_t client_event_HTTP_GET_mashing_raport_handler(esp_http_client_event_handle_t evt);

static void HTTP_GET_mashing_raport(int recipe_id, int stage_id, int bottom_sensor, int top_sensor, uint8_t control_signals)
{
    char url[80];
    sprintf(url,"http://192.168.137.1:9990/mashing/raport/?rid=%d&sid=%d&bs=%d&ts=%d&cs=%d",recipe_id,stage_id,bottom_sensor,top_sensor,control_signals);
    esp_http_client_config_t config_get = {
        //.url = "http://192.168.137.1:9990/mashing/raport/?rid=1&sid=1&bs=20&ts=10&cs=8",
        .url = url,
        .method = HTTP_METHOD_GET,
        .cert_pem = NULL,
        .event_handler = client_event_HTTP_GET_mashing_raport_handler
    };
        
    esp_http_client_handle_t client = esp_http_client_init(&config_get);
    esp_http_client_perform(client);
    esp_http_client_cleanup(client);
}

esp_err_t client_event_HTTP_GET_mashing_procedure_handler(esp_http_client_event_handle_t evt);

static void HTTP_GET_mashing_procedure()
{
    esp_http_client_config_t config_get = {
        .url = "http://192.168.137.1:9990/mashing/procedure",
        .method = HTTP_METHOD_GET,
        .cert_pem = NULL,
        .event_handler = client_event_HTTP_GET_mashing_procedure_handler
    };
        
    esp_http_client_handle_t client = esp_http_client_init(&config_get);
    esp_http_client_perform(client);
    esp_http_client_cleanup(client);
}

//static void HTTP_GET_mashing_procedure();

//esp_err_t client_event_HTTP_GET_mashing_raport_handler(esp_http_client_event_handle_t evt);

//static void HTTP_GET_mashing_raport();
#endif