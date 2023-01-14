set(SOURCES ./Src/main.c)

idf_component_register(SRCS ${SOURCES}
                        INCLUDE_DIRS . Inc
                        REQUIRES 
                            freertos 
                            esp_common 
                            esp_event 
                            nvs_flash
                            esp_adc_cal
                            esp_http_client
                            json
                            )