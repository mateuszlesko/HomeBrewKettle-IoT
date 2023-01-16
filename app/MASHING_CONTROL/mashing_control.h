#ifndef MASHING_CONTROL_H

#define MASHING_CONTROL_H

#include <stdint.h>

#define SENSOR_RELATIVE_ERROR 2 

#define HEATER_GPIO GPIO_NUM_17
#define PUMP_GPIO GPIO_NUM_16

#define REMOTE_PROCESS_PAUSE 0
#define REMOTE_PROCESS_CONTINUE 1
#define REMOTE_PROCESS_FINISH 2

#define CALC_INTER_TO_MIN(INTERVALS) (INTERVALS / 120)

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