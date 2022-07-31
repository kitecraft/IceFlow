/*
 Name:		IceFlow.ino
 Created:	7/19/2022 9:56:21 PM
 Author:	Kitecraft
*/
#include <ESPDateTime.h>
#include <DateTimeTZ.h>
#include "src/IceFlow_Config.h"
#include "src/DisplayManager/DisplayManager.h"
#include "src/DisplayManager/Utilities/DisplayQueue.h"
#include "src/DisplayManager/Utilities/CommandQueue.h"
#include "src/Utilities/ControlCommands.h"
#include "src/Screens/Utilities/InitializeScreens.h"
#include "src/OvenController/OvenController.h"



#include "src/Network/IceNetwork.h"
#include "src/Network/IceWebServer.h"
#include "src/Network/IceOTAManager.h"
bool g_NetworkConnected = false;

TaskHandle_t g_WebServerHandle = nullptr;
TaskHandle_t g_OTAHandle = nullptr;

hw_timer_t* g_ClockTimer = NULL;

static void IRAM_ATTR onTimer() {
    DisplayQueue.QueueKey(suk_DateTime);
}


#include "src/Utilities/PreferencesManager.h"

void setup() {
    Serial.begin(115200);
    Serial.printf("\n\n----- %s v%s -----\n\n", __DEVICE_NAME__, __DEVICE_VERSION__);

    g_ClockTimer = timerBegin(0, 80, true);
    timerAttachInterrupt(g_ClockTimer, &onTimer, true);
    timerAlarmWrite(g_ClockTimer, 1000000, true);
    
    //Init the oven
    OvenManager.Init();

    //SaveProfileFilename("def_prof.json");
    
    //Initialize the DisplayManager
    Display.Init();
    LoadScreensIntoDM();
    DisplayQueue.QueueScreenChange(SN_MAIN_SCREEN);
    //DisplayQueue.QueueScreenChange(SN_OTA_SCREEN);

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
        //Serial.print("Got command queue item: ");
        //Serial.println(command);

        switch (command) {
        case CC_START_OTA:
            DisplayQueue.QueueScreenChange(SN_OTA_SCREEN);
            DisplayQueue.QueueKeyAndValue(suk_Device_Name, __DEVICE_NAME__);
            DisplayQueue.QueueKeyAndValue(suk_Network_Name, GetSsid().c_str());
            DisplayQueue.QueueKeyAndValue(suk_Local_IP_Address, WiFi.localIP().toString().c_str());
            break;
        case CC_REQUEST_NET_STATUS:
            if (g_NetworkConnected) {
                DisplayQueue.QueueKey(suk_Network_Connected);
            }
            else {
                DisplayQueue.QueueKey(suk_Network_Started);
            }
            break;
        case CC_START_TIME_UPATES:
            timerAlarmEnable(g_ClockTimer);
            break;
        case CC_STOP_TIME_UPDATES:
            timerAlarmDisable(g_ClockTimer);
            break;
        case CC_START_TEMPERATURE_STREAM:
            OvenManager.StartTemperatureStream();
            break;
        case CC_STOP_TEMPERATURE_STREAM:
            OvenManager.StopTemperatureStream();
            break;
        default:
            Serial.printf("CommandQueue:  Shouldn't be here. command: %i, Value: \n", command);
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
        g_NetworkConnected = false;
        startLocalNetwork();
        Serial.println("NOT starting OTA handler");
    }
    else {
        g_NetworkConnected = true;
        Serial.println("Starting OTA Handler");
        xTaskCreatePinnedToCore(
            OTAThread,
            "OTA Loop",
            STACK_SIZE,
            nullptr,
            OTA_PRIORITY,
            &g_OTAHandle,
            OTA_CORE);
        SetupDateTime();
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


void SetupDateTime()
{
    // setup this after wifi connected
    // you can use custom timeZone,server and timeout
    // DateTime.setTimeZone(-4);
    //   DateTime.setServer("asia.pool.ntp.org");
    //   DateTime.begin(15 * 1000);
    DateTime.setServer("0.ca.pool.ntp.org");
    DateTime.setTimeZone(TZ_America_Edmonton);
    DateTime.begin();
    if (!DateTime.isTimeValid()) {
        Serial.println("Failed to get time from server.");
    }
    else {
        Serial.printf("UTC    Time:   %s\n", DateTime.formatUTC(DateFormatter::SIMPLE).c_str());
        DisplayQueue.QueueKey(suk_DateTime);
        //Serial.printf("Timestamp is %ld\n", DateTime.now());
    }


    //showTime();
}

void showTime() {
    Serial.printf("TimeZone:      %s\n", DateTime.getTimeZone());
    Serial.printf("Up     Time:   %lu seconds\n", millis() / 1000);
    Serial.printf("Boot   Time:   %ld seconds\n", DateTime.getBootTime());
    Serial.printf("Cur    Time:   %ld seconds\n",
        DateTime.getBootTime() + millis() / 1000);
    Serial.printf("Now    Time:   %ld\n", DateTime.now());
    Serial.printf("OS     Time:   %ld\n", DateTime.osTime());
    Serial.printf("NTP    Time:   %ld\n", DateTime.ntpTime(2 * 1000L));
    // Serial.println();
    Serial.printf("Local  Time:   %s\n",DateTime.format(DateFormatter::SIMPLE).c_str());
    Serial.printf("ISO86  Time:   %s\n", DateTime.toISOString().c_str());
    Serial.printf("UTC    Time:   %s\n",DateTime.formatUTC(DateFormatter::SIMPLE).c_str());
    Serial.printf("UTC86  Time:   %s\n",DateTime.formatUTC(DateFormatter::ISO8601).c_str());
    Serial.printf("Only    Time:   %s\n",DateTime.format(DateFormatter::TIME_ONLY).c_str());
    
    Serial.println("===========");
    time_t t = time(NULL);
    Serial.printf("OS local:     %s", asctime(localtime(&t)));
    Serial.printf("OS UTC:       %s", asctime(gmtime(&t)));
}
