#ifndef GENERAL_H
#define GENERAL_H

#include <iostream>
#include <cstdlib>
#include <ctime>

#define MAX_STAGES 10
#define MAX_MESURUMENTS 6 // 60s : 12 = every 5s one

#define HEATER_PORT 4

#define OIPP 1.5 //optimal increase per period (1.5 C / min)

const char* ssid = "NETIASPOT-2.4GHz-g8qW";
const char* password = "qTJzYf6e";

#define MQTT_SERVER "192.168.100.132"
#define MQTT_PORT 1883

#endif