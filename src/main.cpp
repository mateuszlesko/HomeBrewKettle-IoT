#include <Arduino.h>
#include "../lib/MEASUREMENT/lm35.h"
#include "../lib/GENERAL/general.h"

uint8_t* t_measurements;
uint8_t* optimal_temperatures;
uint8_t liquid_temperatureC;
static uint8_t t_measurements_counter = 0;

hw_timer_t *timer_0 = NULL;

uint8_t control_signals = 0;
//time interrupt handle
void IRAM_ATTR onTimer(){
  liquid_temperatureC = measure_temperature(LM35_PORT);
  if(t_measurements_counter == MAX_MESURUMENT_STATS){
    t_measurements_counter = 0;
    control_signals |= (1<<0);
  }
  if(liquid_temperatureC > 0 && liquid_temperatureC < 102){
    *(t_measurements+t_measurements_counter)=liquid_temperatureC;
  }
  t_measurements_counter++;
  digitalWrite(LED, !digitalRead(LED));
}

void setup() {
  Serial.begin(9600);
  t_measurements = (uint8_t*)calloc(MAX_MESURUMENT_STATS,sizeof(uint8_t*));
  optimal_temperatures = (uint8_t*) calloc(MAX_STAGES,sizeof(uint8_t*));

  pinMode(LED,OUTPUT);
  digitalWrite(LED, 1);
  //setting up time control
  timer_0 = timerBegin(0, 80, true); //setting up timer : select timer number (from 0 to 3), prescaler, count up (true) / count down (false) 
  timerAttachInterrupt(timer_0, &onTimer, true); //setting up time interrupt to timer and interrupt handle func,
  timerAlarmWrite(timer_0, 5000000, true); //setting up timer, time to interrupt, reload - interrupt will be genereated periodically
  timerAlarmEnable(timer_0);
}

void loop() {
  if(control_signals & (1<<0)){
    Serial.println(median(t_measurements,MAX_MESURUMENT_STATS));
    control_signals &= ~(1<<0);
  }
}