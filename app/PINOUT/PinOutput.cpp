#include "Pinout.h"

using namespace PINOUT; 

PinOutput::PinOutput(gpio_num_t _pin)
{
    pin = _pin;
    direction = OUTPUT;
    gpio_set_direction(pin,GPIO_MODE_OUTPUT);
}

PinOutput::PinOutput(const gpio_num_t _pin, enum State _state)
{
    pin = _pin;
    direction = OUTPUT;
    state = _state;
}

PinOutput::~PinOutput()
{
    
}

enum Direction PinOutput::getDirection()
{
    return direction;
}

State PinOutput::getPinState()
{
    return state;
}

void PinOutput::setPinState(enum State _state)
{
    state = _state;
    gpio_set_level(pin,state);
}

gpio_num_t& PinOutput::getPin()
{
    return pin;
}