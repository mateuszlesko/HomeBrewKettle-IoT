#include "http_client.h"

void send_http_get_request(char* url, http_get_request_handler http_handler)
{
     esp_http_client_config_t config_get = {
        .url = url,
        .method = HTTP_METHOD_GET,
        .cert_pem = NULL,
        .event_handler = http_handler
    };
        
    esp_http_client_handle_t client = esp_http_client_init(&config_get);
    esp_http_client_perform(client);
    esp_http_client_cleanup(client);
}


esp_err_t client_event_HTTP_GET_none_return_data_handler(esp_http_client_event_handle_t evt)
{
    ESP_LOGI(HTTP_TAG,"readiness request");
    esp_err_t status = ESP_FAIL;
    switch (evt->event_id)
    {
        case HTTP_EVENT_ERROR:
            ESP_LOGI(HTTP_TAG, "HTTP_EVENT_ERROR");
            break;
        case HTTP_EVENT_ON_CONNECTED:
            ESP_LOGI(HTTP_TAG, "HTTP_EVENT_ON_CONNECTED");
            status = ESP_OK;
            break;
        case HTTP_EVENT_HEADER_SENT:
            ESP_LOGI(HTTP_TAG, "HTTP_EVENT_HEADER_SENT");
            status = ESP_OK;
            break;
        case HTTP_EVENT_ON_HEADER:
            ESP_LOGI(HTTP_TAG, "HTTP_EVENT_ON_HEADER, key=%s, value=%s", evt->header_key, evt->header_value);
            status = ESP_OK;
            break;
        case HTTP_EVENT_ON_DATA:
            ESP_LOGI(HTTP_TAG, "HTTP_EVENT_ON_DATA: %.*s\n", evt->data_len, (char *)evt->data);
            status = ESP_OK;
            break;
        case HTTP_EVENT_ON_FINISH:
            ESP_LOGD(HTTP_TAG, "HTTP_EVENT_ON_FINISH");
            status = ESP_OK;
            break;
        case HTTP_EVENT_DISCONNECTED:
            ESP_LOGI(HTTP_TAG, "HTTP_EVENT_DISCONNECTED");
            status = ESP_OK;
            break;
        default:
            break;
    }
    return status;
}

esp_err_t client_event_HTTP_GET_w_return_data_handler(esp_http_client_event_handle_t evt)
{
    esp_err_t status = ESP_FAIL;
    switch (evt->event_id)
    {
        case HTTP_EVENT_ERROR:
                ESP_LOGI(HTTP_TAG, "HTTP_EVENT_ERROR");
                break;
            case HTTP_EVENT_ON_CONNECTED:
                ESP_LOGI(HTTP_TAG, "HTTP_EVENT_ON_CONNECTED");
                status = ESP_OK;
                break;
            case HTTP_EVENT_HEADER_SENT:
                ESP_LOGI(HTTP_TAG, "HTTP_EVENT_HEADER_SENT");
                status = ESP_OK;
                break;
            case HTTP_EVENT_ON_HEADER:
                ESP_LOGI(HTTP_TAG, "HTTP_EVENT_ON_HEADER, key=%s, value=%s", evt->header_key, evt->header_value);
                status = ESP_OK;
                break;
            case HTTP_EVENT_ON_DATA:
                if (!esp_http_client_is_chunked_response(evt->client)) {
                if (evt->user_data) {
                    memcpy(evt->user_data + http_buffer_len, evt->data, evt->data_len);
                }
                    strcpy(http_data_buffer,(char *)evt->data);
                    ESP_LOGI(HTTP_TAG, "data %s \n",http_data_buffer);
                http_buffer_len += evt->data_len;
            }
                status = ESP_OK;
                break;
            case HTTP_EVENT_ON_FINISH:
                ESP_LOGI(HTTP_TAG, "HTTP_EVENT_ON_FINISH");
                status = ESP_OK;
                break;
            case HTTP_EVENT_DISCONNECTED:
                ESP_LOGI(HTTP_TAG, "HTTP_EVENT_DISCONNECTED");
                status = ESP_OK;
                break;
            default:
                break;
    }
    return status;
}