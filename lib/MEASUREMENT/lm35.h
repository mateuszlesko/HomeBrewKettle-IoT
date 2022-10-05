#ifndef LM35_H
#define LM35_H
//#include <Arduino.h>
extern "C"{
    #define ADC_PROBES 64
    #define ADC_VREF_mV    3300 // in millivolt
    #define ADC_RESOLUTION 4096
    #define LM35_PORT 32
    uint8_t measure_temperature(int port);
    void configure_adc();

}
#endif