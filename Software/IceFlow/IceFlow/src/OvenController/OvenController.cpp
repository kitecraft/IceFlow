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

    _stopButton.attach(STOP_BUTTON, INPUT_PULLUP);
    _stopButton.interval(25);
    _stopButton.setPressedState(LOW);
    //_ovenEnabledMutex = portMUX_INITIALIZER_UNLOCKED;

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

/*
void OvenController::EnableDisableOven(bool state)
{
    EmergencyStop();
    portENTER_CRITICAL(&_ovenEnabledMutex);
    _ovenEnabled = state;
    portEXIT_CRITICAL(&_ovenEnabledMutex);
}
bool OvenController::IsOvenEnabled()
{
    bool ret;
    portENTER_CRITICAL(&_ovenEnabledMutex);
    ret = _ovenEnabled;
    portEXIT_CRITICAL(&_ovenEnabledMutex);
    return ret;
}
*/

void OvenController::CheckConvectionFanStatus()
{
    //If oven is On, and manual mode is on, and Fan is on, 
    //Turn of manual mode when temperature exceeds minium
    if (_heatersOn && _convectionFanOnManual && (_temperaturePrimary > MINIUM_OVEN_TEMPERATURE_FOR_FAN)) {
        EnableConvectionFan(); //Make sure it's on
        _convectionFanOnManual = false;
        return;
    }
    //If the oven and fan are both on, return
    else if (_heatersOn && _convectionFanOn) {
        return;
    }
    //If the oven is on, but the fan isn't, turn the fan on
    else if (_heatersOn && !_convectionFanOn) {
        EnableConvectionFan();
        _convectionFanOnManual = false;
        _convectionFanOnManual = false;
    }
    //If the oven is off, and the fan is on, turn if off if the temperature is below threshold
    //and not in manual mode
    else if (!_heatersOn && (_convectionFanOn && (_temperaturePrimary < MINIUM_OVEN_TEMPERATURE_FOR_FAN) && !_convectionFanOnManual))
    {
        DisableConvectionFan();
        _convectionFanOnManual = false;
        _convectionFanOn = false;
    }
    else if (!_heatersOn && !_convectionFanOn && (_temperaturePrimary > MINIUM_OVEN_TEMPERATURE_FOR_FAN)) {
        EnableConvectionFan();
        _convectionFanOnManual = false;
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
    _convectionFanOnManual = false;
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

}

void OvenController::FetchSecondaryTemperature()
{

}

void OvenController::FetchTemperatures()
{
    /*
    FetchPrimaryTemperature();
    FetchSecondaryTemperature();

    if (_temperaturePrimary >= OVEN_MAXIMUM_TEMPERATURE_PANIC ||
        _temperaturePrimary <= OVEN_MINIUMUM_TEMPERATURE_PANIC ||
        _temperatureSecondary >= OVEN_MAXIMUM_TEMPERATURE_PANIC ||
        _temperatureSecondary <= OVEN_MINIUMUM_TEMPERATURE_PANIC) {
        _ovenStatus = OS_BADNESS;
    }
    */

    if (_testDirection) {
        _temperaturePrimary += 0.001;
        _temperatureSecondary += 0.001;
        if (_temperaturePrimary >= 245) {
            _testDirection = false;
        }

    }
    else {
        _temperaturePrimary -= 0.001;
        _temperatureSecondary -= 0.001;
        if (_temperatureSecondary <= 1) {
            _testDirection = true;
        }
    }
    //delay(100);
}

void OvenController::StartManualHeat(int targetTemperature)
{
    if (_ovenStatus == OS_REFLOW_ACTIVE) {
        return;
    }
    //g_displayQueue.AddScreenChangeToQueue(DISPLAY_SCREEN_TYPE_PREHEAT_SCREEN);
    _manualTargetTemperature = targetTemperature;
    _pidController->Setpoint(targetTemperature);
    _ovenStatus = OS_MANUAL_HEAT_ACTIVE;

    char val[4];
    snprintf(val, 4, "%i", targetTemperature);
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
        /*
        while (!IsOvenEnabled()) {
            vTaskDelay(1);
        }
        */
        _stopButton.update();
        if (_stopButton.pressed()) {
            StopOven();
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

        // DOOM 
        // DONT FORGET TO RENABLE THIS OR ELSE DOOM WILL OCCUR
        CheckConvectionFanStatus();
        // UNCOMMENT THE ABOVE TO AVERT THE DOOM
        // DOOM

        if (_streamTemperatures && _nextTemperatureDisplayUpdate < millis()) {
            char val[7];
            snprintf(val, 7, "%.2f", _temperaturePrimary);
            DisplayQueue.QueueKeyAndValue(suk_PrimaryTemperature, val);

            snprintf(val, 7, "%.2f", _temperatureSecondary);
            DisplayQueue.QueueKeyAndValue(suk_SecondaryTemperature, val);
            _nextTemperatureDisplayUpdate = millis() + TEMPERATURE_DISPLAY_REFRESH_RATE;
        }

        //vTaskDelay(500);
        
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
        Serial.println("Sending: OS_REFLOW_ACTIVE");
        DisplayQueue.QueueKey(suk_Oven_Reflow_On);
        break;
    case OS_CALIBRATION_ACTIVE:
        Serial.println("Sending: OS_CALIBRATION_ACTIVE");
        DisplayQueue.QueueKey(suk_Oven_Calibration_On);
        break;
    default:
        Serial.println("Sending: default");
        DisplayQueue.QueueKey(suk_Oven_Stopped);
        break;
    }
}

OvenController OvenManager;