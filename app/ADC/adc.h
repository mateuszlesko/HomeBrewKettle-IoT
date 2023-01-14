#ifndef ADC_H

#define ADC_H
    #include <stdio.h>
    #include "driver/adc.h"
    #include "esp_adc_cal.h"

    #define ADC_WIDTH 2048

    //static esp_adc_cal_characteristics_t p34_adc1_chars;

    typedef struct 
    {
        adc1_channel_t channel;
        adc_atten_t atten;
        adc_bits_width_t width;
        esp_adc_cal_characteristics_t *p_characts;
        int samples;
    }PinADC1;

    uint32_t measure_mV_method1(PinADC1 *adc);

    void configure_adc1(PinADC1 *adc);

//    PinADC1 adc1 = {
//    .channel = ADC_CHANNEL_7,
//    .atten = ADC_ATTEN_DB_11,
//    .width = ADC_WIDTH_BIT_11,
//    .samples = 32,
//    .p_characts = &p35_adc1_characts
//    };

    //PinADC1 *p_adc1 = &adc1;

#endif