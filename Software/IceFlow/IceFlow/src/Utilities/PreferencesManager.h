#pragma once
#include <Arduino.h>
#include <Preferences.h>

#define WIFI_PREFERENCES_FILENAME "wifi"
#define WIFI_PREFERENCES_SSID_KEY "ssid"
#define WIFI_PREFERENCES_PASWORD_KEY "password"
#define PROFILE_MANAGER_FILENAME "PM"
#define SAVED_PROFILE "CLP" 
#define PROFILE_DEFAULT_FILE "def_prof.json"

#define OVEN_MANAGER_FILENAME "OM"
#define DEFAULT_PID_KP_PREF "DEF_KP" 
#define DEFAULT_PID_KI_PREF "DEF_KI" 
#define DEFAULT_PID_KD_PREF "DEF_KD"
#define PID_DEFAULT_KP 3
#define PID_DEFAULT_KI 3
#define PID_DEFAULT_KD 1.2

#define OM_MANUAL_HEAT_TARGET_KEY "OM_HT"
#define OM_MANUAL_HEAT_TARGET_DEFAULT 200
#define OM_DO_NOT_EXCEED_TEMPERATURE_KEY "OM_DNET"
#define OM_DO_NOT_EXCEED_TEMPERATURE_VALUE 250

static String GetSsid()
{
    String ret;
    Preferences preferences;
    preferences.begin(WIFI_PREFERENCES_FILENAME);
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
    preferences.begin(WIFI_PREFERENCES_FILENAME);
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
    preferences.begin(PROFILE_MANAGER_FILENAME);
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


static void SavePidKP(float kp)
{
    Preferences preferences;
    preferences.begin(OVEN_MANAGER_FILENAME, false);
    preferences.putFloat(DEFAULT_PID_KP_PREF, kp);
    preferences.end();
}

static float GetPidKP()
{
    float ret;
    Preferences preferences;
    preferences.begin(OVEN_MANAGER_FILENAME);
    ret = preferences.getFloat(DEFAULT_PID_KP_PREF, PID_DEFAULT_KP);
    preferences.end();
    return ret;
}

static void SavePidKI(float ki)
{
    Preferences preferences;
    preferences.begin(OVEN_MANAGER_FILENAME, false);
    preferences.putFloat(DEFAULT_PID_KI_PREF, ki);
    preferences.end();
}

static float GetPidKI()
{
    float ret;
    Preferences preferences;
    preferences.begin(OVEN_MANAGER_FILENAME);
    ret = preferences.getFloat(DEFAULT_PID_KI_PREF, PID_DEFAULT_KI);
    preferences.end();
    return ret;
}

static void SavePidKD(float kd)
{
    Preferences preferences;
    preferences.begin(OVEN_MANAGER_FILENAME, false);
    preferences.putFloat(DEFAULT_PID_KD_PREF, kd);
    preferences.end();
}

static float GetPidKD()
{
    float ret;
    Preferences preferences;
    preferences.begin(OVEN_MANAGER_FILENAME);
    ret = preferences.getFloat(DEFAULT_PID_KD_PREF, PID_DEFAULT_KD);
    preferences.end();
    return ret;
}

static void SaveMaualHeatTargetTemperature(int temperature)
{
    Preferences preferences;
    preferences.begin(OVEN_MANAGER_FILENAME, false);
    preferences.putInt(OM_MANUAL_HEAT_TARGET_KEY, temperature);
    preferences.end();
}

static int GetMaualHeatTargetTemperature()
{
    int ret;
    Preferences preferences;
    preferences.begin(OVEN_MANAGER_FILENAME);
    ret = preferences.getInt(OM_MANUAL_HEAT_TARGET_KEY, OM_MANUAL_HEAT_TARGET_DEFAULT);
    preferences.end();
    return ret;
}

static void SaveOvenDoNotExceedTemperature(int temperature)
{
    Preferences preferences;
    preferences.begin(OVEN_MANAGER_FILENAME, false);
    preferences.putInt(OM_DO_NOT_EXCEED_TEMPERATURE_KEY, temperature);
    preferences.end();
}

static int GetOvenDoNotExceedTemperature()
{
    int ret;
    Preferences preferences;
    preferences.begin(OVEN_MANAGER_FILENAME);
    ret = preferences.getInt(OM_DO_NOT_EXCEED_TEMPERATURE_KEY, OM_DO_NOT_EXCEED_TEMPERATURE_VALUE);
    preferences.end();
    return ret;
}