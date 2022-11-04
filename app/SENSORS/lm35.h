#ifndef LM35_H
#define Lm35_H
#include "../ADC/PinADC1.h"
//#pragma once    
namespace SENSORS{
       
    class LM35 {
        public:
        ADC::PinADC1 adc;
        void setADC(ADC::PinADC1 _adc);
        float readTemperature();
    };   
}
#endif