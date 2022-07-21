#pragma once

#define __DEVICE_NAME__ "IceFlow"
#define __DEVICE_VERSION__ "0.1"

#define STACK_SIZE ESP_TASK_MAIN_STACK // Stack size for each new thread
#define WEB_SERVER_PRIORITY (tskIDLE_PRIORITY+2)
#define OTA_PRIORITY (tskIDLE_PRIORITY+2)

#define WEBSERVER_CORE	1
#define OTA_CORE	1

