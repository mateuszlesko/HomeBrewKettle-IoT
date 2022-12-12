#include "http_client.h"

esp_err_t client_event_HTTP_GET_raport_readiness_handler(esp_http_client_event_handle_t evt)
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
//            int mbedtls_err = 0;
//            esp_err_t err = esp_tls_get_and_clear_last_error((esp_tls_error_handle_t)evt->data, &mbedtls_err, NULL);
//            if (err != 0) {
//                ESP_LOGI(HTTP_TAG, "Last esp error code: 0x%x", err);
//                ESP_LOGI(HTTP_TAG, "Last mbedtls failure: 0x%x", mbedtls_err);
//            }
            break;
        default:
            break;
    }
    return status;
}

esp_err_t client_event_HTTP_GET_mashing_procedure_handler(esp_http_client_event_handle_t evt)
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
                //ESP_LOGI(HTTP_TAG, "HTTP_EVENT_ON_DATA: %.*s\n", evt->data_len, (char *)evt->data);
                if (!esp_http_client_is_chunked_response(evt->client)) {
                if (evt->user_data) {
                    memcpy(evt->user_data + http_buffer_len, evt->data, evt->data_len);
                }
//                } else {
//                    if (http_data_buffer == NULL) {
//                        http_data_buffer = (char *) malloc(esp_http_client_get_content_length(evt->client));
//                        http_buffer_len = 0;
//                        if (http_data_buffer == NULL) {
//                            ESP_LOGE(HTTP_TAG, "Failed to allocate memory for output buffer");
//                            return ESP_FAIL;
//                        }
//                    }
                    //memcpy(http_data_buffer, evt->data, evt->data_len);
                    strcpy(http_data_buffer,(char *)evt->data);
                    ESP_LOGI(HTTP_TAG, "data %s\n",http_data_buffer);
//                }
                
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

esp_err_t client_event_HTTP_GET_mashing_raport_handler(esp_http_client_event_handle_t evt)
{
   switch (evt->event_id)
    {
        case HTTP_EVENT_ERROR:
                ESP_LOGI(HTTP_TAG, "HTTP_EVENT_ERROR");
                break;
            case HTTP_EVENT_ON_CONNECTED:
                ESP_LOGI(HTTP_TAG, "HTTP_EVENT_ON_CONNECTED");
                break;
            case HTTP_EVENT_HEADER_SENT:
                ESP_LOGI(HTTP_TAG, "HTTP_EVENT_HEADER_SENT");
                break;
            case HTTP_EVENT_ON_HEADER:
                ESP_LOGI(HTTP_TAG, "HTTP_EVENT_ON_HEADER, key=%s, value=%s", evt->header_key, evt->header_value);
                break;
            case HTTP_EVENT_ON_DATA:
                ESP_LOGI(HTTP_TAG, "HTTP_EVENT_ON_DATA: %.*s\n", evt->data_len, (char *)evt->data);
                break;
            case HTTP_EVENT_ON_FINISH:
                ESP_LOGI(HTTP_TAG, "HTTP_EVENT_ON_FINISH");
                break;
            case HTTP_EVENT_DISCONNECTED:
                ESP_LOGI(HTTP_TAG, "HTTP_EVENT_DISCONNECTED");
                break;
            default:
                break;
    }
    return ESP_OK;
}

esp_err_t client_event_HTTP_GET_mashing_finish_handler(esp_http_client_event_handle_t evt)
{
   switch (evt->event_id)
    {
        case HTTP_EVENT_ERROR:
                ESP_LOGI(HTTP_TAG, "HTTP_EVENT_ERROR");
                break;
            case HTTP_EVENT_ON_CONNECTED:
                ESP_LOGI(HTTP_TAG, "HTTP_EVENT_ON_CONNECTED");
                break;
            case HTTP_EVENT_HEADER_SENT:
                ESP_LOGI(HTTP_TAG, "HTTP_EVENT_HEADER_SENT");
                break;
            case HTTP_EVENT_ON_HEADER:
                ESP_LOGI(HTTP_TAG, "HTTP_EVENT_ON_HEADER, key=%s, value=%s", evt->header_key, evt->header_value);
                break;
            case HTTP_EVENT_ON_DATA:
                ESP_LOGI(HTTP_TAG, "HTTP_EVENT_ON_DATA: %.*s\n", evt->data_len, (char *)evt->data);
                break;
            case HTTP_EVENT_ON_FINISH:
                ESP_LOGI(HTTP_TAG, "HTTP_EVENT_ON_FINISH");
                break;
            case HTTP_EVENT_DISCONNECTED:
                ESP_LOGI(HTTP_TAG, "HTTP_EVENT_DISCONNECTED");
            default:
                break;
    }
    return ESP_OK;
}

static void HTTP_GET_mashing_finish()
{
    esp_http_client_config_t config_get = {
        .url = "http://worldclockapi.com/api/json/utc/now",
        .method = HTTP_METHOD_GET,
        .cert_pem = NULL,
        .event_handler = client_event_HTTP_GET_mashing_finish_handler
    };
        
    esp_http_client_handle_t client = esp_http_client_init(&config_get);
    esp_http_client_perform(client);
    esp_http_client_cleanup(client);
}