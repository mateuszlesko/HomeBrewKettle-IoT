#ifndef MASHING_CONTROL_H

#define MASHING_CONTROL_H

#include <stdint.h>

#define SENSOR_AVG_ABSOLUTE_ERROR (6) 

#define HEATER_GPIO (GPIO_NUM_17)
#define PUMP_GPIO (GPIO_NUM_16)

#define MAX_STAGES_NUM  (8)

#define HEATER_ACTIVE_STATE (0)
#define HEATER_PASSIVE_STATE (1)

#define PUMP_ACTIVE_STATE (0)
#define PUMP_PASSIVE_STATE (1)

#define REMOTE_PROCESS_PAUSE (0)
#define REMOTE_PROCESS_CONTINUE (1)
#define REMOTE_PROCESS_FINISH (2)

#define PERIODS (10)

#define MIN_TO_PERIODS(MIN) ((MIN*60) / PERIODS)
#define CALC_INTER_TO_MIN(INTERVALS) (INTERVALS / 120)

#define mV_TO_C(mV) (mV / 10)

#define MASHING_IS_IN_WORK (0)
#define MASHING_IS_FINISHED (1)

typedef struct{
    uint32_t mashing_id;
    uint32_t recipe_id;
    uint16_t control_signals;
    uint16_t pump;
    unsigned int num_stages;
    unsigned int actual_stage;
    unsigned int actual_time_holding;
    int bottom_sensor;
    int top_sensor;
}T_MashingProcess;

typedef struct{
    int control_signals;
    int pump_work;
}T_RemoteControl;

typedef struct{
    uint8_t heater;
    uint8_t pump;
}T_Actuator;

static unsigned char mashing_finish = MASHING_IS_IN_WORK;
#endif