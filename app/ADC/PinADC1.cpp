#include "PinADC1.h"

using namespace ADC;

void PinADC1::setADCChannel(adc1_channel_t _channel)
{
    channel = _channel;
}

void PinADC1::setADCAttennuation(adc_atten_t _atten)
{
    atten = _atten;
}

void PinADC1::setADCBitsWidth(adc_bits_width_t _bitsWidth)
{
    bitsWidth = _bitsWidth;
}

void PinADC1::calibrateADC()
{
    esp_adc_cal_characterize(ADC_UNIT_1, atten, ADC_WIDTH_BIT_10, 0, &adc1_chars);
    //esp_adc_cal_characterize(channel, atten, ADC_WIDTH_BIT_DEFAULT, 0, &adc1_chars);
}

void PinADC1::configureADC()
{
    adc1_config_width(ADC_WIDTH_BIT_10);

}

uint32_t PinADC1::measure()
{
    uint32_t mV = esp_adc_cal_raw_to_voltage(adc1_get_raw(channel), &adc1_chars);
    //uint32_t val = adc1_get_raw(channel);
    return mV;
}
uint32_t PinADC1::scale(uint32_t adcVal, int a, int b, int c, int d)
{
      /*
        SCALE VALUE FROM ONE RANGE TO ANOTHER:
        a => x => b
        c => y => d
            
        x = measured value from range from a to b
        y = scaled value corresponds to x in range from c to d
        y = (x-a)*(d-c)/(b-a)+c.
       */
    return (adcVal - a) * (d - c) / (b - a) + c;
}
