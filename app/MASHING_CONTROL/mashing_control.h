#ifndef MASHING_CONTROL_H

#define MASHING_CONTROL_H

#include <stdint.h>

#define SENSOR_AVG_ABSOLUTE_ERROR 2 

#define HEATER_GPIO GPIO_NUM_17
#define PUMP_GPIO GPIO_NUM_16

#define MAX_STAGES_NUM  (8)

#define HEATER_ACTIVE_STATE (0)
#define HEATER_PASSIVE_STATE (1)

#define PUMP_ACTIVE_STATE (0)
#define PUMP_PASSIVE_STATE (1)

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

//typedef struct
//{
//    int bottom_temperature;
//    int top_temperature;
//}T_SensorLog;
//
//typedef struct{
//    unsigned char state;
//    unsigned char control_signals;
//}T_MashingProcess;
//
//int convert_mV_to_C(int mV);

/* unit to control process: control acculuators */
//T_MashingProcess control_process(int temperature);

#endif