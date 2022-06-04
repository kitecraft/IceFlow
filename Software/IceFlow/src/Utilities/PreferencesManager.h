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
    ret = preferences.getString(CURRENT_LOADED_PROFILE, String(PROFILE_DEFAULT_FILE));
    if (ret.isEmpty()) {
        ret = String(PROFILE_DEFAULT_FILE);
    }
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

static void SavePidKP(double kp)
{
    Preferences preferences;
    preferences.begin(OVEN_MANAGER_FILENAME, false);
    preferences.putDouble(DEFAULT_PID_KP_PREF, kp);
    preferences.end();
}

static double GetDefaultPidKP()
{
    double ret;
    Preferences preferences;
    preferences.begin(OVEN_MANAGER_FILENAME, false);
    ret = preferences.getDouble(DEFAULT_PID_KP_PREF, PID_DEFAULT_KP);
    preferences.end();
    return ret;
}

static void SavePidKI(double ki)
{
    Preferences preferences;
    preferences.begin(OVEN_MANAGER_FILENAME, false);
    preferences.putDouble(DEFAULT_PID_KI_PREF, ki);
    preferences.end();
}

static double GetDefaultPidKI()
{
    double ret;
    Preferences preferences;
    preferences.begin(OVEN_MANAGER_FILENAME, false);
    ret = preferences.getDouble(DEFAULT_PID_KI_PREF, PID_DEFAULT_KI);
    preferences.end();
    return ret;
}

static void SavePidKD(double kd)
{
    Preferences preferences;
    preferences.begin(OVEN_MANAGER_FILENAME, false);
    preferences.putDouble(DEFAULT_PID_KD_PREF, kd);
    preferences.end();
}

static double GetDefaultPidKD()
{
    double ret;
    Preferences preferences;
    preferences.begin(OVEN_MANAGER_FILENAME, false);
    ret = preferences.getDouble(DEFAULT_PID_KD_PREF, PID_DEFAULT_KD);
    preferences.end();
    return ret;
}

static void SaveDefaultPreHeatTemp(uint16_t temp)
{
    Preferences preferences;
    preferences.begin(OVEN_MANAGER_FILENAME, false);
    preferences.putInt(DEFAULT_PID_KD_PREF, temp);
    preferences.end();
}

static uint16_t GetDefaultPreHeatTemp()
{
    int ret;
    Preferences preferences;
    preferences.begin(OVEN_MANAGER_FILENAME, false);
    ret = preferences.getInt(DEFAULT_PRE_HEAT_PREF, DEFAULT_PRE_HEAT_TEMPERATURE);
    preferences.end();
    return (uint16_t)ret;
}
