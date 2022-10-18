#ifndef PINOUT_H
#define PINOUT_H
#pragma once

#include "driver/gpio.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"
#include "esp_intr_alloc.h"

#include "esp_log.h"

//#include <cassert>
#include <cstdint>
//#include <cstring>
//#include <array>
//#include <bitset>
//#include <mutex>
//#include <string_view>
//#include <type_traits>
//#include <utility>

    
namespace PINOUT{
    
    #define PIN_SETUP_LOG_TAG "PINOUT SETUP"
    #define ESP_INTR_FLAG_DEFAULT 0  /* flag for hardware interrupt caused by pressing confirmation button */ 

    const gpio_num_t& HEATER_PIN = GPIO_NUM_16; /* output PIN - control heater : turn on / off */
    const gpio_num_t& PUMP_PIN = GPIO_NUM_17; /* output PIN - control pump : turn on / off */
    const gpio_num_t& SENSOR1_PIN = GPIO_NUM_34; /* only input PIN - for top LM35 */
    const gpio_num_t& SENSOR2_PIN = GPIO_NUM_35;  /* only input PIN - for buttom LM35 */
    const gpio_num_t& LED_START_PIN = GPIO_NUM_5; /* led - to inform that machine start processing recipe */
    const gpio_num_t& LED_IDLE_PIN = GPIO_NUM_17; /* led - to infrom that machine is power on and waits for recipe */
    const gpio_num_t& LED_EMERGENCY_PIN = GPIO_NUM_16; /* led - to inform about emergency happend */
    const gpio_num_t& CONFIRM_PIN = GPIO_NUM_36; /* only input PIN - for button */
    
    enum State{
        LOW,
        HIGH
    };
    
    enum Direction{
        OUTPUT,
        INPUT
    };
    
//    class GPIOBase{
//        
//        public:
//        GPIOBase(gpio_num_t _pin,enum Direction direction);
//        enum Direction getDirection(); // get info about pin output or input;
//        enum State getPinState();// get current state of pin;
//        gpio_num_t* getPin();
//        
//        protected:
//        State state; 
//        const gpio_num_t* pin;
//        const Direction direction;
//    };
   
    class PinOutput{
        
        private:
        gpio_num_t pin;
        State state;
        Direction direction;
            
        public:
        PinOutput(gpio_num_t& _pin);
        PinOutput( const gpio_num_t& _pin, enum State _state);
        ~PinOutput();
        void setPinState(enum State _state);
        gpio_num_t& getPin();
        Direction getDirection();
        State getPinState();
    };
//    
//    class Pin
    
    
}
#endif