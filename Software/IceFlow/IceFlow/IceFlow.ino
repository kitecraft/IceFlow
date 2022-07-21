/*
 Name:		IceFlow.ino
 Created:	7/19/2022 9:56:21 PM
 Author:	Kitecraft
*/
#include "src/IceFlow_Config.h"
#include "src/DisplayManager/DisplayManager.h"
#include "src/DisplayManager/Utilities/DisplayQueue.h"
#include "src/DisplayManager/Utilities/CommandQueue.h"
#include "src/Utilities/ControlCommands.h"
#include "src/Screens/Utilities/InitializeScreens.h"
#include "src/Utilities/ControlCommands.h"


#include "src/Network/IceNetwork.h"
#include "src/Network/IceWebServer.h"
#include "src/Network/IceOTAManager.h"

TaskHandle_t g_WebServerHandle = nullptr;
TaskHandle_t g_OTAHandle = nullptr;

void setup() {
    Serial.begin(115200);
    Serial.printf("\n\n----- %s v%s -----\n\n", __DEVICE_NAME__, __DEVICE_VERSION__);

    //Initialize the DisplayManager
    Display.Init();
    LoadScreensIntoDM();
    
    DisplayQueue.QueueScreenChange(SN_MAIN_SCREEN);
    StartNetworkStuff();

}

// the loop function runs over and over again until power down or reset
void loop() {
    HandleCommandQueue();
  
}



void HandleCommandQueue()
{
    int key;
    char data[PAYLOAD_MAX_LENGTH];

    while (CommandQueue.GetNextItem(key, data))
    {
        ControlCommands command = static_cast<ControlCommands>(key);
        Serial.print("Got command queue item: ");
        Serial.println(command);

        switch (command) {
        case CC_START_OTA:
            DisplayQueue.QueueScreenChange(SN_OTA_SCREEN);
            DisplayQueue.QueueKeyAndValue(suk_Device_Name, __DEVICE_NAME__);
            DisplayQueue.QueueKeyAndValue(suk_Network_Name, GetSsid().c_str());
            DisplayQueue.QueueKeyAndValue(suk_Local_IP_Address, WiFi.localIP().toString().c_str());
            break;
        default:
            Serial.print("Value: ");
            Serial.println(data);
            break;
        }

        vTaskDelay(1);
    }
}

void StartNetworkStuff()
{
    if (!connectToNetwork())
    {
        startLocalNetwork();
        Serial.println("NOT starting OTA handler");
    }
    else {
        Serial.println("Starting OTA Handler");
        xTaskCreatePinnedToCore(
            OTAThread,
            "OTA Loop",
            STACK_SIZE,
            nullptr,
            OTA_PRIORITY,
            &g_OTAHandle,
            OTA_CORE);
    }

    Serial.println("Starting Webserver");
    xTaskCreatePinnedToCore(
        WebSeverThread,
        "WebServer Loop",
        STACK_SIZE,
        nullptr,
        WEB_SERVER_PRIORITY,
        &g_WebServerHandle,
        WEBSERVER_CORE);
}
