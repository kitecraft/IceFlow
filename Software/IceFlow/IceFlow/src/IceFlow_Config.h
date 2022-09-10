#pragma once

#define __DEVICE_NAME__ "IceFlow"
#define __DEVICE_VERSION__ "1.0"
#define __DEVICE_AUTHOR__ "Kitecraft"
#define __DEVICE_GIT_URL__ "https://github.com/kitecraft/IceFlow"

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
#define STOP_BUTTON_DEBOUNCE_TIME 100
#define OVEN_TEMPERATURE_UPDATE_RATE 100
#define MINIUM_OVEN_TEMPERATURE_REFLOW_START 50
//how often to send the tempeature to the displaymanager
#define TEMPERATURE_DISPLAY_REFRESH_RATE 100

//PID Data
#define PID_DATA_SPIFFS_FILE_DIRECTORY "/d/"

//Profile Manager
#define PROFILE_SPIFFS_FILE_DIRECTORY "/p/"
#define MAXIMUM_PROFILE_FILE_NAME_LENGTH 8
#define FILENAME_EXTENSION ".json"

//Thermocouplers
#define THERMOCOUPLER_CLK 12
#define THERMOCOUPLER_DO 14
#define THERMOCOUPLER_PRIMARY_CS 27
#define THERMOCOUPLER_SECONDARY_CS 26

//Hardware definintions
#define RELAY_OFF LOW
#define RELAY_ON HIGH

#define PRIMARY_TEMPERATURE_COLOR TFT_RED
#define SEC_TEMPERATURE_COLOR TFT_ORANGE
#define TER_TEMPERATURE_COLOR TFT_GREEN

//how often the screen updates it's graphs
#define UPDATE_GRAPH_RATE 250
