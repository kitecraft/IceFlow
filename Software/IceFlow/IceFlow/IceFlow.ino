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
#include "src/Utilities/PreferencesManager.h"

#include "src/Network/IceNetwork.h"
#include "src/Network/IceWebServer.h"
#include "src/Network/IceOTAManager.h"


TaskHandle_t g_networkStartupHandle = nullptr;

TaskHandle_t g_WebServerHandle = nullptr;
TaskHandle_t g_OTAHandle = nullptr;

hw_timer_t* g_ClockTimer = NULL;
volatile bool SendDatetimeUpdate = false;

static void IRAM_ATTR onTimer() {
    SendDatetimeUpdate = true;
}

void HandleCommandQueue()
{
    int key;
    char data[PAYLOAD_MAX_LENGTH];

    while (CommandQueue.GetNextItem(key, data))
    {
        ControlCommands command = static_cast<ControlCommands>(key);

        switch (command) {
        case CC_START_OTA:
            OvenManager.StopOven();
            DisplayQueue.QueueScreenChange(SN_OTA_SCREEN);
            DisplayQueue.QueueKeyAndValue(suk_Device_Name, __DEVICE_NAME__);
            DisplayQueue.QueueKeyAndValue(suk_Network_Name, GetSsid().c_str());
            DisplayQueue.QueueKeyAndValue(suk_Local_IP_Address, WiFi.localIP().toString().c_str());
            break;
        case CC_REQUEST_NET_STATUS:
            switch (g_NetworkStatus) {
            case IF_NETWORK_CONNECTED:
                DisplayQueue.QueueKey(suk_Network_Connected);
                break;
            case IF_NETWORK_STARTED:
                DisplayQueue.QueueKey(suk_Network_Started);
            }
            break;
        case CC_REQUEST_NET_NAME:
            switch (g_NetworkStatus) {
            case IF_NETWORK_CONNECTED:
                DisplayQueue.QueueKeyAndValue(suk_Network_Name, GetSsid().c_str());
                break;
            case IF_NETWORK_STARTED:
                DisplayQueue.QueueKeyAndValue(suk_Network_Name, __DEVICE_NAME__);
                break;
            }
            break;
        case CC_REQUEST_NET_IP:
            switch (g_NetworkStatus) {
            case IF_NETWORK_CONNECTED:
                DisplayQueue.QueueKeyAndValue(suk_IP_Address, WiFi.localIP().toString().c_str());
                break;
            case IF_NETWORK_STARTED:
                DisplayQueue.QueueKeyAndValue(suk_IP_Address, WiFi.softAPIP().toString().c_str());
                break;
            }
            break;
        case CC_REQUEST_MAC_ADDRESS:
            DisplayQueue.QueueKeyAndValue(suk_Mac_Address, WiFi.macAddress().c_str());
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
        case CC_START_MANUAL_HEAT:
            OvenManager.StartManualHeat(atoi(data));
            break;
        case CC_REQUEST_OVEN_STATUS:
            OvenManager.SendStatus();
            break;
        case CC_START_AUTOTUNE:
            OvenManager.StartAutoTune(atof(data));
            break;
        case CC_START_REFLOW:
            if (!OvenManager.StartReflowSession()) {
                OvenManager.EmergencyStopOven();
            };
            break;
        case CC_STOP_OVEN:
            OvenManager.StopOven();
            break;
        case CC_OVEN_RESET_PIDS:
            OvenManager.ResetPIDs();
            break;
        case CC_DELETE_TOUCH_CALIBRATION:
            Display.DeleteTouchCalibrationFile();
            break;
        case CC_UPDATE_DO_NOT_EXCEED_TEMPERATURE:
            OvenManager.UpdateDoNotExceedTemperature();
            break;
        default:
            Serial.printf("CommandQueue:  Shouldn't be here. command: %i, Value: \n", command);
            Serial.println(data);
            break;
        }

        vTaskDelay(1);
    }
}

static void IRAM_ATTR SetupDateTime()
{
    DateTime.setServer("0.ca.pool.ntp.org");
    DateTime.setTimeZone(TZ_America_Edmonton);
    DateTime.begin();
    if (!DateTime.isTimeValid()) {
        Serial.println("Failed to get time from server.");
    }
    else {
        Serial.printf("UTC    Time:   %s\n", DateTime.formatUTC(DateFormatter::SIMPLE).c_str());
    }
}

static void IRAM_ATTR StartNetworkStuff(void* pvParameters)
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

    vTaskDelete(g_networkStartupHandle);
}

void setup() {
    Serial.begin(115200);
    Serial.printf("\n\n----- %s v%s -----\n\n", __DEVICE_NAME__, __DEVICE_VERSION__);

    //Initialize the DisplayManager
    Display.Init();
    LoadScreensIntoDM();
    DisplayQueue.QueueScreenChange(SN_MAIN_SCREEN);

    delay(1000);

    //Init the oven
    OvenManager.Init();

    xTaskCreate(
        StartNetworkStuff,
        "NetworkSetup",
        STACK_SIZE,
        (void*)1,
        tskIDLE_PRIORITY,
        &g_networkStartupHandle);

    g_ClockTimer = timerBegin(0, 80, true);
    timerAttachInterrupt(g_ClockTimer, &onTimer, true);
    timerAlarmWrite(g_ClockTimer, 1000000, true);

}

void loop() {
    HandleCommandQueue();

    if (SendDatetimeUpdate) {
        DisplayQueue.QueueKey(suk_DateTime);
        SendDatetimeUpdate = false;
    }
}