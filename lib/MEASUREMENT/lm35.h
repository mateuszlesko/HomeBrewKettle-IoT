#ifndef LM35_H
#define LM35_H

#include <Arduino.h>

#define ADC_VREF_mV    3300.0 // in millivolt
#define ADC_RESOLUTION 4096.0
#define LM35_PORT 32

uint8_t measure_temperature(int port);
#endif