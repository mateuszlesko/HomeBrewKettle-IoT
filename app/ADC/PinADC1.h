#ifndef PIN_ADC_H
#define PIN_ADC_H

#include <stdio.h>
#include <stdlib.h>
#pragma once
#include "driver/adc.h"
#include "esp_adc_cal.h"

namespace ADC{

    #define DEFAULT_VREF    3300        //Use adc2_vref_to_gpio() to obtain a better estimate

    const adc1_channel_t SENSOR1_ADC = ADC1_CHANNEL_6; /* only input PIN - for top LM35 */

    const adc1_channel_t SENSOR2_ADC = ADC1_CHANNEL_7;  /* only input PIN - for buttom LM35 */


    class PinADC1{
  
        public:
        adc1_channel_t channel;
        adc_atten_t   atten;
        adc_bits_width_t bitsWidth;
        esp_adc_cal_characteristics_t adc1_chars;
    
        void setADCChannel(adc1_channel_t _channel);
        void setADCAttennuation(adc_atten_t _atten);
        void setADCBitsWidth(adc_bits_width_t _bitsWidth);
        void calibrateADC();
        void configureADC();
        uint32_t measure();
        
    };
}


#endif