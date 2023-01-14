# Test group: choosing prescaler value
## What's prescaler?
Prescaler is a divider for timer, the purpose is to slow down timer counting by dividing clock frequency by selected value.

## Goal: 
The goal is to set prescaler value for ESP32 timer,  timer counter should overflow during __1 min__.
<<<<<<< HEAD
=======
## Tests cases:

>>>>>>> a7b3513bb6b34af888b0dd8813c3e9e4c16e4598

## How test was conducted?

The test was conducted by using ESP-IDF fuction 
`` esp_err_t timer_get_counter_time_sec(timer_group_tgroup_num, timer_idx_ttimer_num, double *time) ``

<<<<<<< HEAD
=======

>>>>>>> a7b3513bb6b34af888b0dd8813c3e9e4c16e4598
## Test enviroment:

__Device:__  NODE MCU ESP32 - WROOM U - 80 MHz

__Framework:__ ESP-IDF

__Selected Clock Params__:
- GROUP :  0
- ID : 0
- 64 Bits

## Test cases:
### test #1 :
__Prescaler value:__ 380

__Result expected:__ +-60 secs

__Actual result:__ +- 23 secs ❌

### test #2 :
__Prescaler value:__ 488

__Result expected:__ +-60 secs

__Actual result:__ +- 30 secs ❌

### test #3 :
__Prescaler value:__ 600

__Result expected:__ +-60 secs

__Actual result:__ +- 37sec ❌

### test #4 : 
__Prescaler value:__  930 

__Result expected:__  +-60 secs

__Actual result:__   57.87 secs ✔️

### test #5 :
__Prescaler value:__ 982

__Result expected:__ +-60 secs

__Actual result:__ +- 61 secs ✔️

## Test result:
The best prescaler value will be between 930 and 982.

## Validation tests:
### test 1:
__Prescaler value:__ 972

__Result expected:__ +-60 secs

__Actual result:__ +- 60 secs ✔️

## Test result:
<<<<<<< HEAD
The best prescaler value will be between 930 and 982 - validated. ✔️
=======
The best prescaler value will be between 930 and 982 - validated. ✔️
>>>>>>> a7b3513bb6b34af888b0dd8813c3e9e4c16e4598
