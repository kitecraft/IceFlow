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
#define PID_DEFAULT_KI 0.24
#define PID_DEFAULT_KD 1.2

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


static void SavePidKP(double kp)
{
    Preferences preferences;
    preferences.begin(OVEN_MANAGER_FILENAME, false);
    preferences.putDouble(DEFAULT_PID_KP_PREF, kp);
    preferences.end();
}

static double GetPidKP()
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

static double GetPidKI()
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

static double GetPidKD()
{
    double ret;
    Preferences preferences;
    preferences.begin(OVEN_MANAGER_FILENAME, false);
    ret = preferences.getDouble(DEFAULT_PID_KD_PREF, PID_DEFAULT_KD);
    preferences.end();
    return ret;
}