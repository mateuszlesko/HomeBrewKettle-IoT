#ifndef GENERAL_H
#define GENERAL_H

#include <iostream>
#include <cstdlib>
#include <ctime>

#define MAX_STAGES 10
#define MAX_MESURUMENT_STATS 14 // 90s : 14 = every 5s one

#define LED 32

void sort_array(uint8_t *arr,uint8_t length);
uint8_t  median(uint8_t arr[14],int length);
#endif