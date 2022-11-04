#include "LM35.h"

using namespace SENSORS;

void LM35::setADC(ADC::PinADC1 _adc)
{
    adc = _adc;
}

float LM35::readTemperature()
{
    float data = (double)adc.measure() * 0.01;
    return 2.5 * data;
}