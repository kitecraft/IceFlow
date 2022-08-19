#pragma once
#include <Arduino.h>
#include "OvenController.h"

class AutoTune
{
public:
    float targetTemperature = 150;

    float currentTemp = 0;
    int cycles = 0;
    bool heating = true;

    unsigned long temp_millis = millis();
    unsigned long t1 = temp_millis;
    unsigned long t2 = temp_millis;
    long t_high;
    long t_low;

    long bias = 127;
    long d = 127;
    float Ku, Tu;
    float Kp, Ki, Kd;
    float maxTemp = 20, minTemp = 20;

    AutoTune() { }
    AutoTune(float target) {
        Init(target);
    }

    void Init(float target) {
        targetTemperature = target;
        currentTemp = 0;
        cycles = 0;
        heating = true;

        temp_millis = millis();
        t1 = temp_millis;
        t2 = temp_millis;
        t_high;
        t_low;

        bias = 127;
        d = 127;
        Ku, Tu;
        Kp, Ki, Kd;
        maxTemp = 20, minTemp = 20;
    }
};

