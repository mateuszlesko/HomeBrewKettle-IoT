#include <Arduino.h>
#include <WiFi.h>
#include "../lib/MEASUREMENT/lm35.h"
#include "../lib/GENERAL/general.h"
#include "../lib/MEASUREMENT/mesurement_stats.h"
#include "../lib/PubSub/PubSubClient.h"

uint8_t* t_measurements;

uint8_t* optimal_temperatures;

uint8_t prev_liquid_temperatureC;

static uint8_t t_measurements_counter = 0;

static uint8_t stage_index = 0;

hw_timer_t *timer_0 = NULL;

uint8_t control_signals = 0;

WiFiClient espClient;

PubSubClient mqttClient(espClient);

char messageString[8];

//time interrupt handle
void IRAM_ATTR onTimer(){
  uint8_t liquid_temperatureC = measure_temperature(LM35_PORT);

  if(t_measurements_counter == MAX_MESURUMENTS){
    t_measurements_counter = 0;
    control_signals |= (1<<0);
  }

  if(liquid_temperatureC > 0 && liquid_temperatureC < 102){
    *(t_measurements+t_measurements_counter)=liquid_temperatureC;
  }

  t_measurements_counter++;
}

void setup() {
  Serial.begin(9600);
  t_measurements = (uint8_t*)calloc(MAX_MESURUMENTS,sizeof(uint8_t*));
  optimal_temperatures = (uint8_t*) calloc(1,sizeof(uint8_t*));
  optimal_temperatures[0] = 20;

  pinMode(HEATER_PORT,OUTPUT);
  digitalWrite(HEATER_PORT, 1);
  
   Serial.println("");
  Serial.print("connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid,password);

  while(WiFi.status() != WL_CONNECTED){
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("Connected");
  Serial.println("IP:");
  Serial.println(WiFi.localIP());

  mqttClient.setServer(MQTT_SERVER,MQTT_PORT);
   while(!mqttClient.connected()){
    if(mqttClient.connect("ESP8266Client")){
      break;
    }
    else{ 
      Serial.println("trying to connect");
      delay(1000);
    }
  }
  
  //setting up time control
  timer_0 = timerBegin(0, 80, true); //setting up timer : select timer number (from 0 to 3), prescaler, count up (true) / count down (false) 
  timerAttachInterrupt(timer_0, &onTimer, true); //setting up time interrupt to timer and interrupt handle func,
  timerAlarmWrite(timer_0, 5000000, true); //setting up timer, time to interrupt, reload - interrupt will be genereated periodically
  timerAlarmEnable(timer_0);

  // temperature probe #0
  prev_liquid_temperatureC = measure_temperature(LM35_PORT);
}

void loop() {
 
  while(!mqttClient.connected()){
    if(mqttClient.connect("ESP8266Client")){
      break;
    }
    else{ 
      Serial.println("trying to connect");
      delay(1000);
    }
  }
  mqttClient.loop();
  
  if(control_signals & (1<<0)){
    uint8_t estimated_T = calculate_median(t_measurements,MAX_MESURUMENTS);
    Serial.print("Temperatura [C]:");
    Serial.println(estimated_T);
    if( (float)estimated_T - (float)prev_liquid_temperatureC > 2.6){
      control_signals &= ~(1 << 1); // increase per minute to high : heater off
      digitalWrite(HEATER_PORT,0);
    }
    if( estimated_T <= optimal_temperatures[stage_index]){
       control_signals |= (1 << 1); // increase per minute to high : heater on
       digitalWrite(HEATER_PORT,1);
    }
    if(estimated_T > optimal_temperatures[stage_index]){
      control_signals &= ~(1<<1);
      digitalWrite(HEATER_PORT,0);
    }
    dtostrf(estimated_T, 1, 2, messageString);
    mqttClient.publish("mashtum/temperature",messageString);
    control_signals &= ~(1<<0);
  }
}