set(SOURCES wifi.c)

idf_component_register(SRCS ${SOURCES}
                        INCLUDE_DIRS "."
                        REQUIRES 
                        nvs_flash
                        esp_wifi)