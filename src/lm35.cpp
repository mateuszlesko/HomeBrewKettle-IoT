//#include <Arduino.h>
#include <stdint.h>
#include "driver/adc.h"
#include "esp_adc_cal.h"
#include<stdio.h>
#include "../lib/MEASUREMENT/lm35.h"

// uint8_t measure_temperature(int port){
//     int adc_value = analogRead(port);
//     float mili_volts = adc_value * (5000.0 / 4096.0);
//     return uint8_t(mili_volts / 10);
// }


extern "C"{

void configure_adc(){
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(ADC1_CHANNEL_4,ADC_ATTEN_DB_0);
}

uint8_t measure_temperature(int port){
    uint32_t adc_value;
    for(int i = 0; i < ADC_PROBES; i++){
        adc_value +=  adc1_get_raw((adc1_channel_t)ADC1_CHANNEL_4);
    }
    uint32_t mili_volts = adc_value * (ADC_VREF_mV / ADC_RESOLUTION);
    return (uint8_t)(mili_volts / 10);
}
}