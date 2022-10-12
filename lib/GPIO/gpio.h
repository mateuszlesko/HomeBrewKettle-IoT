#ifndef GPIO_H
#define GPIO_H
#pragma once
#include <cstdint>
#include "driver/gpio.h"
    
namespace GPIO
{
    #define HEATER_PIN 16
    #define PUMP_PIN 17
    #define SENSOR1_PIN 34
    #define SENSOR2_PIN 35
    
    enum state{
        LOW,
        HIGH
    };
    
    enum direction{
        OUTPUT,
        INPUT
    };
    
    class GPIOBase{
        public:
        enum state getState() = 0;
        enum direction getDirection() = 0;
        void setPinValue(enum state state) = 0;
        protected:
        enum state state;
        enum direction direction;    
    };
}

#endif