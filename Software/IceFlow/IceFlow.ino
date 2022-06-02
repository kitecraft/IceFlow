/*
 Name:		IceFlow.ino
 Created:	5/26/2022 10:26:28 PM
 Author:	Kitecraft
*/
#include <Adafruit_MAX31855.h>
#include "./src/IceFlow_Config.h"
#include "./src/Network/IceNetwork.h"
#include "./src/Network/IceOTAManager.h"
#include "./src/Network/IceWebServer.h"
#include "src/Utilities/CommandQueue.h"
#include "src/OvenController/OvenController.h"
#include "src/DisplayManager/DisplayManager.h"
#include "src/ProfileManager/ProfileManager.h"

OvenController g_ovenController;
TaskHandle_t g_ovenControllerHandle = nullptr;
void IRAM_ATTR OvenControllerThread(void*)
{
    g_ovenController.Run();
}

ProfileManager g_profileManager;

TaskHandle_t g_webServerHandle = nullptr;
TaskHandle_t g_OTAHandle = nullptr;

DisplayQueue g_displayQueue;
CommandQueue g_commandQueue;

DisplayManager g_displayManager;
TaskHandle_t g_displayManagerHandle = nullptr;
void IRAM_ATTR DisplayManagerThread(void*)
{
    g_displayManager.Run();
}

//Adafruit_MAX31855 g_probeA(PROBE_CLK, PROBE_A_CS_PIN, PROBE_MISO);
//Adafruit_MAX31855 g_probeB(PROBE_CLK, PROBE_B_CS_PIN, PROBE_MISO);
//unsigned long g_nextTemperatureUpdate = 0;


bool setupComplete = true;
void setup() {
    Serial.begin(115200);
    Serial.printf("\n\n----- %s v%s -----\n\n", __DEVICE_NAME__, __DEVICE_VERSION__);
    
    g_ovenController.Init();
    xTaskCreatePinnedToCore(
        OvenControllerThread,
        "OvenController",
        STACK_SIZE,
        nullptr,
        OVEN_CONTROLLER_PRIORITY,
        &g_ovenControllerHandle,
        OVEN_CONTROLLER_CORE
    );

    g_displayManager.Init();
    xTaskCreatePinnedToCore(
        DisplayManagerThread,
        "DisplayManager",
        STACK_SIZE,
        nullptr,
        DISPLAY_MANAGER_PRIORITY,
        &g_displayManagerHandle,
        DISPLAY_MANAGER_CORE
    );

    /*
    if (!InitializeTemperatureProbes())
    {
        Serial.println("Failed to initalize all temperature probes. As such, I refuse to start");
        while (true) {}
    }
    */

    StartNetworkStuff();

    g_displayQueue.AddScreenChangeToQueue(DISPLAY_SCREEN_TYPE_MAIN);
    delay(1000);
}


void loop() {
    HandleCommandQueue();
    /*
    if (!GetTemperatures()) {
        Serial.println("A critical failure has occured.  Shut it all down now!");
    }
    */
}

void HandleCommandQueue()
{
    while (!g_commandQueue.IsQueueEmpty())
    {
        COMMAND_QUEUE_ITEM currItem = g_commandQueue.GetNextItem();
        switch (currItem.command) {
        case STARSIDE_CMD_BEGIN_OTA:
            break;
        case STARSIDE_CMD_RESTART:
            ESP.restart();
            break;
        case STARSIDE_CMD_DELETE_TOUCH_CALIBRATION:
            g_displayManager.DeleteTouchCalibrationFile();
            break;
        case STARSIDE_CMD_START_MANUAL_PREHEAT:
            g_ovenController.StartManualPreHeat(currItem.value.toInt());
        default:
            break;
        }
        yield();
    }
}

/*
bool GetTemperatures()
{
    if (g_nextTemperatureUpdate > millis()) {
        return true;
    }

    Serial.printf("\n---\nInternal Temps  A = %f | B = %f\n", g_probeA.readInternal(), g_probeB.readInternal());

    double probeA_C = g_probeA.readCelsius();
    double probeB_C = g_probeB.readCelsius();
    if (isnan(probeA_C)) {
        Serial.println("Something wrong with g_probeA!");
        return false;
    }
    if (isnan(probeB_C)) {
        Serial.println("Something wrong with g_probeB!");
        return false;
    }

    Serial.printf("A = %f | B = %f\n", probeA_C, probeB_C);

    g_nextTemperatureUpdate = millis() + TEMPERATURE_REFRESH_RATE;
    return true;
}

bool InitializeTemperatureProbes()
{
    delay(500);
    Serial.println("Initializing sensors...");
    if (!g_probeA.begin()) {
        Serial.println("g_probeA ERROR.");
        return false;
    }
    if (!g_probeB.begin()) {
        Serial.println("g_probeB ERROR.");
        return false;
    }

    return true;
}
*/

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
        &g_webServerHandle,
        WEBSERVER_CORE);

    //delay(5000);
}