#ifndef ADC_H

#define ADC_H
    #include "freertos/FreeRTOS.h"
    #include "driver/adc.h"
    #include "esp_adc_cal.h"
    
    #define ADC_WIDTH 2048

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

#endif