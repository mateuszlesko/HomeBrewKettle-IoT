#include "LM35.h"

using namespace SENSORS;

void LM35::setADC(ADC::PinADC1 _adc)
{
    adc = _adc;
}

int LM35::readTemperature()
{
    int data = (int)(adc.measure() * 0.01);
    return 3 * data;
}