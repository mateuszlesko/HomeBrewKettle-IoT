#include "adc.h"

void configure_adc1(PinADC1 *adc)
{
    adc1_config_channel_atten(adc->channel, adc->atten);
    adc1_config_width(adc->width);
    esp_adc_cal_characterize(ADC_UNIT_1, adc->atten, adc->width, 0, adc->p_characts);
}

uint32_t measure_mV_method1(PinADC1 *adc)
{
    uint32_t raw = adc1_get_raw(adc->channel);
    uint32_t mV = esp_adc_cal_raw_to_voltage(raw,adc->p_characts);
    return mV;
}
