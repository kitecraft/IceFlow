#pragma once
#include <Arduino.h>
#include "SPIFFS.h"
#include <WebServer.h>
#include <ESPmDNS.h>
#include "../IceFlow_Config.h"
#include "../Utilities/CommandQueue.h"
#include "../Utilities/IceFS.h"

static WebServer server(80);
extern CommandQueue g_commandQueue;

void IRAM_ATTR WebSeverThread(void*)
{
    if (MDNS.begin(__DEVICE_NAME__)) {
        Serial.println("MDNS responder started");
    }

    server.on("/", []() {
        String webPage = IceFS_ReadFile(WEBPAGE_NETWORK);
        webPage.replace("%{__DEVICE_NAME__}%", __DEVICE_NAME__);
        webPage.replace("%{ERRORTEXT}%", "");
        webPage.replace("%{NETWORKNAME}%", GetSsid());
        server.send(200, "text/html", webPage);
        });

    server.on("/reboot", []() {
        server.sendHeader("Location", "/");
        server.send(303);
        ESP.restart();
        });

    server.on("/net", []() {
        String webPage = IceFS_ReadFile(WEBPAGE_NETWORK);
        webPage.replace("%{__DEVICE_NAME__}%", __DEVICE_NAME__);
        webPage.replace("%{ERRORTEXT}%", "");
        webPage.replace("%{NETWORKNAME}%", GetSsid());
        server.send(200, "text/html", webPage);
        });

    server.on("/setupNet", []() {
        String webPage = IceFS_ReadFile(WEBPAGE_NETWORK);
        if (server.arg("password") != server.arg("passwordConfirm"))
        {
            webPage.replace("%{ERRORTEXT}%", "Passwords did not match.<br>Changes not saved.");
        }
        else {
            SetSsid(server.arg("networkName"));
            SetSsidPassword(server.arg("password"));
            webPage.replace("%{ERRORTEXT}%", "Changes have been saved.");
        }
        webPage.replace("%{__DEVICE_NAME__}%", __DEVICE_NAME__);
        webPage.replace("%{NETWORKNAME}%", GetSsid());
        server.send(200, "text/html", webPage);
        });

    server.on("/DeleteCalibration", []() {
        g_commandQueue.AddItemToQueue(STARSIDE_CMD_DELETE_TOUCH_CALIBRATION);
        String webPage = IceFS_ReadFile(WEBPAGE_NETWORK);
        webPage.replace("%{__DEVICE_NAME__}%", __DEVICE_NAME__);
        webPage.replace("%{ERRORTEXT}%", "");
        webPage.replace("%{NETWORKNAME}%", GetSsid());
        server.send(200, "text/html", webPage);
        });

    server.begin();
    Serial.println("HTTP server started");

    while (true)
    {
        server.handleClient();
        vTaskDelay(5);
    }
}
