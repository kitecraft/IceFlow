#include "OvenController.h"
#include "../Utilities/PreferencesManager.h"
#include "../DisplayManager/Utilities/DisplayQueue.h"
#include "../Screens/Utilities/ScreenUpdateKeys.h"


bool OvenController::Init()
{
    pinMode(SSR_OVEN_HEATERS, OUTPUT);
    digitalWrite(SSR_OVEN_HEATERS, RELAY_OFF);

    pinMode(RELAY_CONVECTION_FAN, OUTPUT);
    digitalWrite(RELAY_CONVECTION_FAN, RELAY_OFF);

    pinMode(STOP_BUTTON, INPUT_PULLUP);

    Serial.println("Initializing sensor A...");
    _primaryTemperatureSensor.selectHSPI();
    _primaryTemperatureSensor.begin( THERMOCOUPLER_PRIMARY_CS);
    Serial.print("Status: ");
    Serial.println(_primaryTemperatureSensor.getStatus());

    _kp = GetPidKP();
    _ki = GetPidKI();
    _kd = GetPidKD();
    
    Serial.print("PID: ");
    Serial.print(_kp);
    Serial.print(" - ");
    Serial.print(_ki);
    Serial.print(" - ");
    Serial.println(_kd);
    _pidController = new PID_v2(_kp, _ki, _kd, PID::Direct);
    _pidController->SetSampleTime(20);

    FetchPrimaryTemperature();
    _pidController->Start(_temperaturePrimary, 0, 0);

    xTaskCreatePinnedToCore(
        OvenControllerTask,
        "OvenManager",
        OVEN_CONTROLLER_TASK_STACK_SIZE ,
        nullptr,
        OVEN_CONTROLLER_PRIORITY,
        &_taskHandle,
        OM_CORE
    );

    return true;
}

void OvenController::CheckConvectionFanStatus()
{
    //If the oven and fan are both on, return
    if (_heatersOn && _convectionFanOn) {
        return;
    }

    //If the oven is on, but the fan isn't, turn the fan on
    else if (_heatersOn && !_convectionFanOn) {
        EnableConvectionFan();
    }

    //If the oven is off, and the fan is on, turn if off if the temperature is below threshold
    //and not in manual mode
    else if (!_heatersOn && (_convectionFanOn && (_temperaturePrimary < MINIUM_OVEN_TEMPERATURE_FOR_FAN)))
    {
        DisableConvectionFan();
        _convectionFanOn = false;
    }
    else if (!_heatersOn && !_convectionFanOn && (_temperaturePrimary > MINIUM_OVEN_TEMPERATURE_FOR_FAN)) {
        EnableConvectionFan();
        return;
    }
}

void OvenController::EnableConvectionFan()
{
    digitalWrite(RELAY_CONVECTION_FAN, RELAY_ON);
    _convectionFanOn = true;
}

void OvenController::DisableConvectionFan()
{
    digitalWrite(RELAY_CONVECTION_FAN, RELAY_OFF);
    _convectionFanOn = false;
}

void OvenController::EnableOvenHeaters()
{
    digitalWrite(SSR_OVEN_HEATERS, RELAY_ON);
    if (!_convectionFanOn) {
        EnableConvectionFan();
    }
    _heatersOn = true;
    DisplayQueue.QueueKey(suk_Oven_Heaters_On);
    
}

void OvenController::DisableOvenHeaters()
{
    digitalWrite(SSR_OVEN_HEATERS, RELAY_OFF);
    _heatersOn = false;
    DisplayQueue.QueueKey(suk_Oven_Heaters_Off);
}

void OvenController::StopOven()
{
    Serial.println("\n-----\nEMERGENCY STOP\n-----\n");
    _ovenStatus = OS_IDLE;
    _reflowPhase = RP_NOT_ACTIVE;
    DisableOvenHeaters();
    DisableConvectionFan();
    DisplayQueue.QueueKey(suk_Oven_Stopped);
    
}

void OvenController::FetchPrimaryTemperature()
{
    if (_nextTemperatureUpdate < millis()) {
        _nextTemperatureUpdate = millis() + OVEN_TEMPERATURE_UPDATE_RATE;
        double prevTemp = _temperaturePrimary;
        int ret = _primaryTemperatureSensor.read();
        if (ret != STATUS_OK) {
            Serial.println("Something wrong with thermocouple!");
        }
        else {
            _temperaturePrimary = (double)_primaryTemperatureSensor.getTemperature();
            if (_temperaturePrimary == 0 || isnan(_temperaturePrimary)) {
                Serial.println("ISNAN error!");
                _temperaturePrimary = prevTemp;
            }
        }
    }

}

void OvenController::FetchSecondaryTemperature()
{
    _temperatureSecondary = 10;
}

void OvenController::FetchTemperatures()
{
    FetchPrimaryTemperature();
    FetchSecondaryTemperature();

    /*
    if (_temperaturePrimary >= OVEN_MAXIMUM_TEMPERATURE_PANIC ||
        _temperaturePrimary <= OVEN_MINIUMUM_TEMPERATURE_PANIC ||
        _temperatureSecondary >= OVEN_MAXIMUM_TEMPERATURE_PANIC ||
        _temperatureSecondary <= OVEN_MINIUMUM_TEMPERATURE_PANIC) {
        _ovenStatus = OS_BADNESS;
    }


    //if (_testDirection) {
    if(_heatersOn) {
        _temperaturePrimary += 0.001;
        _temperatureSecondary += 0.001;
        //if (_temperaturePrimary >= 245) {
        //    _testDirection = false;
        //}

    }
    else {
        _temperaturePrimary -= 0.001;
        _temperatureSecondary -= 0.001;
        //if (_temperatureSecondary <= 1) {
        //    _testDirection = true;
        //}
        
        if (_temperaturePrimary < 15) {
            _temperaturePrimary = 15;
        }
        if (_temperatureSecondary < 5) {
            _temperatureSecondary = 5;
        }
    }
    */



}

void OvenController::StartManualHeat(int targetTemperature)
{
    if (_ovenStatus == OS_REFLOW_ACTIVE) {
        return;
    }
    _manualTargetTemperature = targetTemperature;

    _pidController->Setpoint(_manualTargetTemperature);
    _ovenStatus = OS_MANUAL_HEAT_ACTIVE;

    char val[4];
    snprintf(val, 4, "%i", _manualTargetTemperature);
    DisplayQueue.QueueKeyAndValue(suk_Oven_Manual_On, val);
}

void OvenController::StartReflowSession(String profileFileName)
{
    /*
    if (!g_profileManager.GetProfile(profileFileName, &_profile)) {
        return;
    }
    */

    DisplayQueue.QueueKey(suk_Oven_Reflow_On);
}

void OvenController::HandleOvenHeatersWithPID()
{
    const double output = _pidController->Run(_temperaturePrimary);
    if (output == 0 && _heatersOn)
    {
        DisableOvenHeaters();
    }
    else if (output > 0 && !_heatersOn) {
        EnableOvenHeaters();
    }
}

void OvenController::HandleReflowSession()
{
    HandleOvenHeatersWithPID();

}

void OvenController::Run()
{
    while (true) {

        if (digitalRead(STOP_BUTTON) == LOW) {
            if (millis() - STOP_BUTTON_DEBOUNCE_TIME > _lastButtonPress) {
                StopOven();
                _lastButtonPress = millis();
            }
        }

        FetchTemperatures();

        switch (_ovenStatus) {
        case OS_IDLE:
            break;
        case OS_MANUAL_HEAT_ACTIVE:
            HandleOvenHeatersWithPID();
            break;
        case OS_REFLOW_ACTIVE:
            HandleReflowSession();
            break;
        case OS_CALIBRATION_ACTIVE:
            break;
        default:
            StopOven();
            Serial.println("\n***\nSomething has gone horribly wrong.\nPlease unplug the oven.\nThe warranty is void\n***\n");
            break;
        }

        CheckConvectionFanStatus();

        if (_streamTemperatures && _nextTemperatureDisplayUpdate < millis()) {
            char val[7];
            snprintf(val, 7, "%.2f", _temperaturePrimary);
            DisplayQueue.QueueKeyAndValue(suk_PrimaryTemperature, val);

            //snprintf(val, 7, "%.2f", _temperatureSecondary);
            snprintf(val, 7, "%.2f", _temperaturePrimary - 5);
            DisplayQueue.QueueKeyAndValue(suk_SecondaryTemperature, val);
            _nextTemperatureDisplayUpdate = millis() + TEMPERATURE_DISPLAY_REFRESH_RATE;
        }
        
        vTaskDelay(1);
    }
}

void OvenController::SendStatus()
{
    switch (_ovenStatus) {
    case OS_IDLE:
        DisplayQueue.QueueKey(suk_Oven_Stopped);
        break;
    case OS_MANUAL_HEAT_ACTIVE:
        char val[4];
        snprintf(val, 4, "%i", _manualTargetTemperature);
        DisplayQueue.QueueKeyAndValue(suk_Oven_Manual_On, val);
        break;
    case OS_REFLOW_ACTIVE:
        DisplayQueue.QueueKey(suk_Oven_Reflow_On);
        break;
    case OS_CALIBRATION_ACTIVE:
        DisplayQueue.QueueKey(suk_Oven_Calibration_On);
        break;
    default:
        DisplayQueue.QueueKey(suk_Oven_Stopped);
        break;
    }

    if (_heatersOn) {
        DisplayQueue.QueueKey(suk_Oven_Heaters_On);
    }
    else {
        DisplayQueue.QueueKey(suk_Oven_Heaters_Off);
    }
}

OvenController OvenManager;