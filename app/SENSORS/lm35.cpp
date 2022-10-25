#include "LM35.h"

using namespace SENSORS;

void LM35::setADC(ADC::PinADC1 _adc)
{
    adc = _adc;
}

double LM35::readTemperature()
{
    double data = (double)adc.measure() * 0.01;
    return data;
}