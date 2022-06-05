#pragma once
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <SPIFFS.h>
#include "../IceFlow_Config.h"
#include "../Utilities/CommandQueue.h"
#include "../DisplayManager/Utilities/DisplayQueue.h"

static void IRAM_ATTR OTAThread(void*)
{
    static int OTA_Completion_Percentage = 0;
    ArduinoOTA.setHostname(__DEVICE_NAME__);
    ArduinoOTA
        .onStart([]() {
            String type;
            if (ArduinoOTA.getCommand() == U_FLASH)
            {
                type = "sketch";
            }
            else // U_SPIFFS
            {
                type = "filesystem";
            }
            g_commandQueue.AddItemToQueue(STARSIDE_CMD_BEGIN_OTA);
            Serial.println("\n\n---STARTING OTA FIRMWARE UPDATE---");
            delay(1000);
            SPIFFS.end();
            })

        .onEnd([]() {
            Serial.println("OTA FIRMWARE UPDATE completed");
            g_displayQueue.AddItemToQueue(DISPLAY_COMMAND_UPDATE_VALUE, DISPLAY_UPDATE_KEY_OTA_COMPLETE, "");
            delay(1000);
            })

        .onProgress([](unsigned int progress, unsigned int total) {
            int percentage = progress / (total / 100);
            if (percentage != OTA_Completion_Percentage)
            {
                OTA_Completion_Percentage = percentage;
                Serial.println("OTA FIRMWARE UPDATE PROGRESS: " + String(percentage) + "%");
                g_displayQueue.AddItemToQueue(DISPLAY_COMMAND_UPDATE_VALUE, DISPLAY_UPDATE_KEY_OTA_PROGRESS_UPDATE, String(percentage));
            }
        })

        .onError([](ota_error_t error) {
            Serial.printf("Error[%u]: ", error);
            if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
            else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
            else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
            else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
            else if (error == OTA_END_ERROR) Serial.println("End Failed");
        });

        ArduinoOTA.begin();

        while (true) {
            ArduinoOTA.handle();
            delay(1);
        }
}