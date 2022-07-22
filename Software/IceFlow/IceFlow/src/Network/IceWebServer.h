#pragma once
#include <Arduino.h>
#include "SPIFFS.h"
#include <WebServer.h>
#include "../Utilities/IceFS.h"
#include "../DisplayManager/DisplayManager.h"

static WebServer server(80);

void IRAM_ATTR WebSeverThread(void*)
{
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
        server.sendHeader("Location", "/");
        server.send(303);
        Display.DeleteTouchCalibrationFile();
        });

    server.begin();
    Serial.println("HTTP server started");

    while (true)
    {
        server.handleClient();
        vTaskDelay(5);
    }
}
