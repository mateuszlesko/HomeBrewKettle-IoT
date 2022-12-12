#ifndef JSON_HELPER_H

#define JSON_HELPER_H

#include <stdlib.h>
#include "cJSON.h"
#include "../MASHING_CONTROL/mashing_control.h"


void deserialize_json_to_mashing_recipe(char* json, Mashing *p_mashing, int* temperatures, int* durations);

#endif