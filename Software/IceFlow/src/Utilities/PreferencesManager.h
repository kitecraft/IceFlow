#pragma once
#include <Arduino.h>
#include "../IceFlow_Config.h"
#include <Preferences.h>

static String GetSsid()
{
    String ret;
    Preferences preferences;
    preferences.begin(WIFI_PREFERENCES_FILENAME, false);
    ret = preferences.getString(WIFI_PREFERENCES_SSID_KEY, "");
    preferences.end();
    return ret;
}

static void SetSsid(String s)
{
    Preferences preferences;
    preferences.begin(WIFI_PREFERENCES_FILENAME, false);
    preferences.putString(WIFI_PREFERENCES_SSID_KEY, s);
    preferences.end();
}

static String GetSsidPassword()
{
    String ret;
    Preferences preferences;
    preferences.begin(WIFI_PREFERENCES_FILENAME, false);
    ret = preferences.getString(WIFI_PREFERENCES_PASWORD_KEY, "");
    preferences.end();

    return ret;
}

static void SetSsidPassword(String s)
{
    Preferences preferences;
    preferences.begin(WIFI_PREFERENCES_FILENAME, false);
    preferences.putString(WIFI_PREFERENCES_PASWORD_KEY, s);
    preferences.end();
}

static String GetSavedProfile()
{
    String ret = "";
    Preferences preferences;
    preferences.begin(PROFILE_MANAGER_FILENAME, false);
    ret = preferences.getString(CURRENT_LOADED_PROFILE, "");
    preferences.end();
    return ret;
}


static void SaveProfileName(String profileFileName)
{
    Preferences preferences;
    preferences.begin(PROFILE_MANAGER_FILENAME, false);
    preferences.putString(CURRENT_LOADED_PROFILE, profileFileName);
    preferences.end();
}