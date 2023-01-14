#include "wifi.h"

uint8_t mac[6] = {0x70, 0xB3, 0xD5, 0x6A, 0x02, 0x01};

void wifi_event_handler(void *event_handler_arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
        switch (event_id)
        {
        case WIFI_EVENT_STA_START:
            printf("WiFi connecting ... \n");
            break;
        case WIFI_EVENT_STA_CONNECTED:
            printf("WiFi connected ... \n");
            break;
        case WIFI_EVENT_STA_DISCONNECTED:
            printf("WiFi lost connection ... \n");
            ESP_ERROR_CHECK(esp_wifi_connect());
            break;
        case IP_EVENT_STA_GOT_IP:
            printf("WiFi got IP ... \n\n");
            break;
        default:
            break;
        }
}

esp_err_t init_wifi_connection()
{
    nvs_flash_init();
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    
    esp_netif_t *netif_handle = esp_netif_create_default_wifi_sta();
    esp_netif_set_hostname(netif_handle, HOSTNAME);
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    
    esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, wifi_event_handler, NULL);
    esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, wifi_event_handler, NULL);
    
    esp_wifi_set_mac(WIFI_IF_STA, mac);
    wifi_config_t conf = {};
    
    strcpy((char*)conf.sta.ssid, SSID);
    strcpy((char*)conf.sta.password, PASSWORD);
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &conf));
    ESP_ERROR_CHECK(esp_wifi_start());
    ESP_ERROR_CHECK(esp_wifi_connect());
    
    return ESP_OK; 
}