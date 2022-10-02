#include <Arduino.h>
#include "../lib/MEASUREMENT/lm35.h"

uint8_t measure_temperature(int port){
    int adc_value = analogRead(port);
    float mili_volts = adc_value * (5000.0 / 4096.0);
    return uint8_t(mili_volts / 10);
}

