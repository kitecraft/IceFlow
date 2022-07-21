#pragma once
#include <Arduino.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include "../Utilities/PreferencesManager.h"
#include "../DisplayManager/Utilities/DisplayQueue.h"
#include "../Screens/Utilities/ScreenUpdateKeys.h"


// NETWORK
#define NETWORK_WAIT_TIME_PER_TRY 30 //seconds
#define NETWORK_TRY_TIMES 3

//Webpages
#define WEBPAGE_NETWORK "/net.htm"

static bool connectToNetwork()
{
    WiFi.hostname(__DEVICE_NAME__);
    WiFi.mode(WIFI_STA);
    //SetSsid("");
    //SetSsidPassword("");

    if (GetSsid().equals(""))
    {
        return false;
    }
    //DisplayQueue.QueueKeyAndValue(suk_Network_Name, GetSsid().c_str());
    Serial.print("Connecting Network: ");

    int tryCount = 1;
    bool connected = false;
    while (tryCount <= NETWORK_TRY_TIMES) {
        Serial.printf("Try #%i to connect to network '%s'\n", tryCount, GetSsid());
        WiFi.begin(GetSsid().c_str(), GetSsidPassword().c_str(), 1);
        int waitCount = 0;
        while (waitCount < NETWORK_WAIT_TIME_PER_TRY * 2) {
            if (WiFi.status() == WL_CONNECTED) {
                Serial.print("Connected to network: " + GetSsid() + " - ");
                Serial.println(WiFi.localIP());
                if (MDNS.begin(__DEVICE_NAME__)) {
                    Serial.println("MDNS responder started");
                }
                //DisplayQueue.QueueKeyAndValue(suk_Local_IP_Address, WiFi.localIP().toString().c_str());
                DisplayQueue.QueueKey(suk_Network_Connected);
                return true;
            }
            delay(500);
            waitCount++;
        }
        tryCount++;
    }
    return false;
}

static void startLocalNetwork()
{
    Serial.print("Failed to connect to network.  Createing local: ");
    Serial.print(__DEVICE_NAME__);
    Serial.print(" - ");

    IPAddress IP = WiFi.softAP(__DEVICE_NAME__);
    Serial.println(WiFi.softAPIP());
    if (MDNS.begin(__DEVICE_NAME__)) {
        Serial.println("MDNS responder started");
    }
    //DisplayQueue.QueueKeyAndValue(suk_Network_Name, __DEVICE_NAME__);
    //DisplayQueue.QueueKeyAndValue(suk_Local_IP_Address, WiFi.softAPIP().toString().c_str());
    DisplayQueue.QueueKey(suk_Network_Started);
}