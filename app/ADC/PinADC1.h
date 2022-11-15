#ifndef PIN_ADC_H
#define PIN_ADC_H

#include <stdio.h>
#include <stdlib.h>
#pragma once
#include "driver/adc.h"
#include "esp_adc_cal.h"

namespace ADC{

    const adc1_channel_t SENSOR1_ADC = ADC1_CHANNEL_6; /* only input PIN - for top LM35 */

    const adc1_channel_t SENSOR2_ADC = ADC1_CHANNEL_7;  /* only input PIN - for buttom LM35 */

    #define ADC_ATTEN_DB_11_LOW 150
    #define ADC_ATTEN_DB_11_HIGH 2450
    
    class PinADC1{
  
        public:
        adc1_channel_t channel;
        adc_atten_t   atten;
        adc_bits_width_t bitsWidth;
        esp_adc_cal_characteristics_t adc1_chars;
        int samples;
        
        void setADCChannel(adc1_channel_t _channel);
        void setADCAttennuation(adc_atten_t _atten);
        void setADCBitsWidth(adc_bits_width_t _bitsWidth);
        void calibrateADC();
        void configureADC();
        void setSamples(int s);
        double measure();
    };
}


#endif