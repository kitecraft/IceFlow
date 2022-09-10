#pragma once
#include <Arduino.h>
#include "OvenController.h"
#include "../IceFlow_Config.h"

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

    long d = PID_AUTOTUNE_D_VALUE;
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

        d = PID_AUTOTUNE_D_VALUE;
        maxTemp = 20;
        minTemp = 20;
    }
};

