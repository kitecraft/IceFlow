#pragma once
#include <WiFi.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <SPIFFS.h>
#include "../Utilities/ControlCommands.h"
#include "../DisplayManager/Utilities/CommandQueue.h"
#include "../Screens/Utilities/ScreenUpdateKeys.h"
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
            CommandQueue.QueueCommand(CC_START_OTA);
            Serial.println("\n\n---STARTING OTA FIRMWARE UPDATE---");
            //delay(1000);
            SPIFFS.end();
            })

        .onEnd([]() {
            Serial.println("OTA FIRMWARE UPDATE completed");
            DisplayQueue.QueueKey(suk_OTA_Complete);
            delay(1000);
            })

        .onProgress([](unsigned int progress, unsigned int total) {
            int percentage = progress / (total / 100);
            if (percentage != OTA_Completion_Percentage)
            {
                OTA_Completion_Percentage = percentage;
                Serial.println("OTA FIRMWARE UPDATE PROGRESS: " + String(percentage) + "%");
                DisplayQueue.QueueKeyAndValue(suk_OTA_Progress, String(percentage).c_str());
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