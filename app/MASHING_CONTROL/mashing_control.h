#ifndef MASHING_CONTROL_H

#define MASHING_CONTROL_H

#include <stdint.h>

#define ACCURACY 2 

typedef struct{
    uint16_t control_signals;
    int mashing_id;
    int recipe_id;
    int num_stages;
    int actutal_stage;
    int bottom_sensor;
    int top_sensor;
}Mashing;


//extern Mashing *p_mashing;

//extern int *mashing_temperatures;
//extern int *mashing_temperature_holdings;
extern int actual_stage;
extern int actual_time_holding;
#endif