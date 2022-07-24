#pragma once
#include <Arduino.h>
#include <Preferences.h>

#define WIFI_PREFERENCES_FILENAME "wifi"
#define WIFI_PREFERENCES_SSID_KEY "ssid"
#define WIFI_PREFERENCES_PASWORD_KEY "password"
#define PROFILE_MANAGER_FILENAME "PM"
#define SAVED_PROFILE "CLP" 
#define PROFILE_DEFAULT_FILE "def_prof.json"

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

static String GetSavedProfileFileName()
{
    String ret = "";
    Preferences preferences;
    preferences.begin(PROFILE_MANAGER_FILENAME, false);
    ret = preferences.getString(SAVED_PROFILE, PROFILE_DEFAULT_FILE);
    if (ret.isEmpty()) {
        ret = PROFILE_DEFAULT_FILE;
    }
    preferences.end();
    return ret;
}

static void SaveProfileFilename(String profileFileName)
{
    Preferences preferences;
    preferences.begin(PROFILE_MANAGER_FILENAME, false);
    preferences.putString(SAVED_PROFILE, profileFileName);
    preferences.end();
}
