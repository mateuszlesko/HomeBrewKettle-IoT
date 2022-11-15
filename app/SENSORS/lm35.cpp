#include "LM35.h"

using namespace SENSORS;

void LM35::setADC(ADC::PinADC1 _adc)
{
    adc = _adc;
}

double LM35::readTemperature()
{
    double val = adc.measure();
    double scale = (val / 2048.0) * 3300.0;
    double tempC = scale / 10;
//    int data = (int)(adc.measure() * 0.01);
//    return 3 * data;
    return tempC;
}