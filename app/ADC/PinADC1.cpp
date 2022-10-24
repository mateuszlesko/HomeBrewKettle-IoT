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
    esp_adc_cal_characterize(ADC_UNIT_1, atten, ADC_WIDTH_BIT_DEFAULT, 0, &adc1_chars);
    //esp_adc_cal_characterize(channel, atten, ADC_WIDTH_BIT_DEFAULT, 0, &adc1_chars);
}

void PinADC1::configureADC()
{
    adc1_config_width(ADC_WIDTH_BIT_DEFAULT);

}

uint32_t PinADC1::measure()
{
    uint32_t mV = esp_adc_cal_raw_to_voltage(adc1_get_raw(channel), &adc1_chars);
    return mV;
}
