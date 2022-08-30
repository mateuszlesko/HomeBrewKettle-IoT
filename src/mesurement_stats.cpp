#include "../lib/MEASUREMENT/mesurement_stats.h"


#include "../lib/MEASUREMENT/mesurement_stats.h"

void sort_array(uint8_t *arr,uint8_t length){
    for(uint8_t i = 0; i < length; i++){
        for(uint8_t j = i + 1; j < length; j++){
        if(arr[j] < arr[i]){
            uint8_t tmp = arr[i];
            arr[i] = arr[j];
            arr[j] = tmp;
        }
        }
    }
}

uint8_t  median(uint8_t arr[14],int length){
    return length %2 == 0 ? uint8_t((arr[6]+arr[7]) / 2) : arr[7];
}