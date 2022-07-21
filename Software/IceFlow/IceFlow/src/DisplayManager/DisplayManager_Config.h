#pragma once

//comment out the following if not using touch
#define DM_TOUCH_ENABLED

//The full path for the touch screen calibration data
#define CALIBRATION_FILENAME "/_calData_"

//The touch screens touch debounce time
#define TOUCH_DEBOUNCE_TIME 100

//Comment out the followling line if not useing DMA
#define DM_USE_DMA

//Select the ESP32 core to run the DisplayManager on
#define DM_CORE 0

//Set the priority of the DisplayManager task
#define DM_PRIORITY (tskIDLE_PRIORITY + 4)

//Set the stack size for the task which runs the Display.
//You may need to increase this if you get a stack watchdog
//error.  
#define DM_TASK_STACK_SIZE (4096+512) 

//Use this to set the TFT screen rotation.
//You can of course change the orientation at 
//any time in your own screen classes
#define DM_SCREEN_ROTATION 1

//Maximum size of payload
//Wold be nice to change payload
//to a dynmically allocated pointer
#define PAYLOAD_MAX_LENGTH 64



