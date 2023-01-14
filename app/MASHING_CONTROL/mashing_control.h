#ifndef MASHING_CONTROL_H

#define MASHING_CONTROL_H

#include <stdint.h>

#define SENSOR_RELATIVE_ERROR 2 

#define HEATER_GPIO GPIO_NUM_4

typedef struct{
    uint16_t control_signals;
    int mashing_id;
    int recipe_id;
    int num_stages;
    int actual_stage;
    int actual_time_holding;
    int bottom_sensor;
    int top_sensor;
}Mashing;

typedef struct{
    int control_signals;
}RemoteControl;

#endif