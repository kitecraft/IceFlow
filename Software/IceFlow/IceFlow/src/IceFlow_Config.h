#pragma once

#define __DEVICE_NAME__ "IceFlow"
#define __DEVICE_VERSION__ "0.1"

#define STACK_SIZE ESP_TASK_MAIN_STACK // Stack size for each new thread
#define WEB_SERVER_PRIORITY (tskIDLE_PRIORITY+2)
#define OTA_PRIORITY (tskIDLE_PRIORITY+2)

#define WEBSERVER_CORE	1
#define OTA_CORE	1

//OVEN
#define OM_CORE 1
#define MINIUM_OVEN_TEMPERATURE_FOR_FAN 50
#define OVEN_CONTROLLER_PRIORITY tskIDLE_PRIORITY+4
#define OVEN_CONTROLLER_TASK_STACK_SIZE (4096+512) 
#define RELAY_CONVECTION_FAN 32
#define SSR_OVEN_HEATERS 33
#define STOP_BUTTON 22
//how often to send the tempeature to the displaymanager
#define TEMPERATURE_DISPLAY_REFRESH_RATE 100

//Hardware definintions
#define RELAY_OFF LOW
#define RELAY_ON HIGH

#define PRIMARY_TEMPERATURE_COLOR TFT_RED
#define SEC_TEMPERATURE_COLOR TFT_ORANGE

//how often the screen updates it's graphs
#define UPDATE_GRAPH_RATE 250
