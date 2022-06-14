/*
 Name:		IceFlow.ino
 Created:	5/26/2022 10:26:28 PM
 Author:	Kitecraft
*/

/*include both of these*/
#include <esp32/spiram.h>
#include <esp32-hal-psram.h>
/**/

#include "src/IceFlow_Config.h"
#include "src/Network/IceNetwork.h"
#include "src/Network/IceOTAManager.h"
#include "src/Network/IceWebServer.h"
#include "src/Utilities/CommandQueue.h"
#include "src/OvenController/OvenController.h"
#include "src/DisplayManager/DisplayManager.h"
#include "src/Utilities/PreferencesManager.h"
#include <Bounce2.h>
#include <ESPDateTime.h>
#include <DateTimeTZ.h>

OvenController g_ovenController;
TaskHandle_t g_ovenControllerHandle = nullptr;
void IRAM_ATTR OvenControllerThread(void*)
{
    g_ovenController.Run();
}

TaskHandle_t g_webServerHandle = nullptr;
TaskHandle_t g_OTAHandle = nullptr;

//DisplayQueue g_displayQueue;
//CommandQueue g_commandQueue;

DisplayManager g_displayManager;
TaskHandle_t g_displayManagerHandle = nullptr;
void IRAM_ATTR DisplayManagerThread(void*)
{
    g_displayManager.Run();
}

Bounce2::Button g_stopOvenBtn;

//Adafruit_MAX31855 g_probeA(PROBE_CLK, PROBE_A_CS_PIN, PROBE_MISO);
//Adafruit_MAX31855 g_probeB(PROBE_CLK, PROBE_B_CS_PIN, PROBE_MISO);
//unsigned long g_nextTemperatureUpdate = 0;


bool setupComplete = true;
void setup() {
    /*
        This seems to fix a crash in TFT_eSPI sprite ps_calloc()
        Basically, allocate ALL of the avaialable PSRAM then free it.
        Any PSRAM not allocated here will cause a crash in TFT_eSPI
        if it tries to ps_calloc() it.
    */
    esp_spiram_init();
    vTaskDelay(1);
    psramInit();
    vTaskDelay(1);
    /*
    uint8_t* ptr8 = (uint8_t*)ps_calloc(ESP.getMaxAllocPsram(), sizeof(uint8_t));
    uint8_t* ptr8b = (uint8_t*)ps_calloc(ESP.getMaxAllocPsram(), sizeof(uint8_t));
    free(ptr8);
    free(ptr8b);
    ptr8 = (uint8_t*)ps_malloc(ESP.getMaxAllocPsram());
    ptr8b = (uint8_t*)ps_malloc(ESP.getMaxAllocPsram());
    free(ptr8);
    free(ptr8b);
    */

    Serial.begin(115200);
    Serial.printf("\n\n----- %s v%s -----\n\n", __DEVICE_NAME__, __DEVICE_VERSION__);
    Serial.println((String)"getPsramSize : " + ESP.getPsramSize());
    Serial.println((String)"Memory available in PSRAM : " + ESP.getFreePsram());
    Serial.println((String)"getMaxAllocPsram : " + ESP.getMaxAllocPsram());
    
    /*
    Serial.println((String)"getHeapSize : " + ESP.getHeapSize());
    Serial.println((String)"getFreeHeap : " + ESP.getFreeHeap());
    Serial.println((String)"getMinFreeHeap : " + ESP.getMinFreeHeap());
    Serial.println((String)"getMaxAllocHeap : " + ESP.getMaxAllocHeap());
    */

    g_stopOvenBtn.attach(EMERGENCY_STOP_BTN_PIN, INPUT_PULLUP);
    g_stopOvenBtn.interval(BUTTON_DEBOUNCE_TIME);
    g_stopOvenBtn.setPressedState(LOW);
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
    HandleButtonPress();
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
            g_ovenController.DisableOven();
            g_displayQueue.AddScreenChangeToQueue(DISPLAY_SCREEN_TYPE_OTA_SCREEN);
            g_displayQueue.AddItemToQueue(DISPLAY_COMMAND_UPDATE_VALUE, DISPLAY_UPDATE_KEY_WIFI_SSID, GetSsid());
            g_displayQueue.AddItemToQueue(DISPLAY_COMMAND_UPDATE_VALUE, DISPLAY_UPDATE_KEY_WIFI_LOCAL_IP, WiFi.localIP().toString());
            g_displayQueue.AddItemToQueue(DISPLAY_COMMAND_UPDATE_VALUE, DISPLAY_UPDATE_KEY_OTA_STARTING, "");
            break;
        case STARSIDE_CMD_RESTART:
            ESP.restart();
            break;
        case STARSIDE_CMD_DELETE_TOUCH_CALIBRATION:
            g_displayManager.DeleteTouchCalibrationFile();
            break;
        case STARSIDE_CMD_START_MANUAL_PREHEAT:
            g_ovenController.StartManualPreHeat(currItem.value.toInt());
            break;
        case STARSIDE_CMD_START_REFLOW:
            g_ovenController.StartReflowSession(currItem.value);
            break;
        default:
            break;
        }
        yield();
    }
}

void HandleButtonPress()
{
    g_stopOvenBtn.update();
    if (g_stopOvenBtn.pressed()) {
        g_ovenController.EmergencyStop();
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

        SetupDateTime();
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
        Serial.printf("Date Now is %s\n", DateTime.toISOString().c_str());
        Serial.printf("Timestamp is %ld\n", DateTime.now());
    }

    showTime();
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
    Serial.printf("Local  Time:   %s\n",
        DateTime.format(DateFormatter::SIMPLE).c_str());
    Serial.printf("ISO86  Time:   %s\n", DateTime.toISOString().c_str());
    Serial.printf("UTC    Time:   %s\n",
        DateTime.formatUTC(DateFormatter::SIMPLE).c_str());
    Serial.printf("UTC86  Time:   %s\n",
        DateTime.formatUTC(DateFormatter::ISO8601).c_str());

    Serial.println("===========");
    time_t t = time(NULL);
    Serial.printf("OS local:     %s", asctime(localtime(&t)));
    Serial.printf("OS UTC:       %s", asctime(gmtime(&t)));
}
