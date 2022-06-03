#include "OvenController.h"
#include "../Utilities/PreferencesManager.h"

bool OvenController::Init()
{
    pinMode(SSR_OVEN_HEATERS, OUTPUT);
    digitalWrite(SSR_OVEN_HEATERS, RELAY_OFF);

    pinMode(RELAY_CONVECTION_FAN, OUTPUT);
    digitalWrite(RELAY_CONVECTION_FAN, RELAY_OFF);
    _kp = GetDefaultPidKP();
    _ki = GetDefaultPidKI();
    _kd = GetDefaultPidKD();

    Serial.print("PID: ");
    Serial.print(_kp);
    Serial.print(" - ");
    Serial.print(_ki);
    Serial.print(" - ");
    Serial.println(_kd);

    _pidController = new PID_v2(_kp, _ki, _kd, PID::Direct);
    _pidController->SetSampleTime(20);
    _pidController->Start(_temperaturePrimary, 0, 20.0);
    return true;
}

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
}

void OvenController::DisableOvenHeaters()
{
    digitalWrite(SSR_OVEN_HEATERS, RELAY_OFF);
    _heatersOn = false;
}

void OvenController::EmergencyStop() 
{
    Serial.println("\n-----\nEMERGENCY STOP\n-----\n");
    _ovenStatus = OS_IDLE;
    _reflowPhase = RP_NOT_ACTIVE;
    DisableOvenHeaters(); 
    DisableConvectionFan();
}

void OvenController::FetchPrimaryTemperature()
{

}

void OvenController::FetchSecondaryTemperature()
{

}

void OvenController::FetchTemperatures()
{
    FetchPrimaryTemperature();
    FetchSecondaryTemperature();
}

void OvenController::StartManualPreHeat(uint16_t targetTemperature)
{
    if (_ovenStatus == OS_REFLOW_ACTIVE) {
        return;
    }
    Serial.print("Starting pre-heat: ");
    Serial.println(targetTemperature);
    _pidController->Setpoint(targetTemperature);
    _ovenStatus = OS_MANUAL_HEAT;
}

void OvenController::StartReflowSession(String profileFileName)
{
    if (!g_profileManager.GetProfile(profileFileName, _profile)) {
        return;
     }

    // _pidController->Start(_temperaturePrimary, 0, (double) pre_heat_temperature);
}

void OvenController::HandleManualPreHeat()
{
    //get temperature
    if (_testDirection){
        _temperaturePrimary++;
        if (_temperaturePrimary >= 180) {
            _testDirection = false;
        }
    }
    else {
        _temperaturePrimary--;
        if (_temperaturePrimary <= 25) {
            _testDirection = true;
        }
    }

    Serial.print("Temp: ");
    Serial.print(_temperaturePrimary);
    const double output = _pidController->Run(_temperaturePrimary);
    if (output == 0 && _heatersOn)
    {
        DisableOvenHeaters();
    }
    else if (output > 0 && !_heatersOn) {
        EnableOvenHeaters();
    }

    Serial.print("  | Output: ");
    Serial.println(output);
    delay(500);
}

void OvenController::HandleReflowSession()
{

}

void OvenController::Run()
{
    while (true) {
        FetchTemperatures();

        switch (_ovenStatus) {
        case OS_IDLE:
            break;
        case OS_MANUAL_HEAT:
            HandleManualPreHeat();
            break;
        case OS_REFLOW_ACTIVE:
            HandleReflowSession();
            break;
        default:
            Serial.println("Something has gone horribly wrong");
            EmergencyStop();
            break;
        }
        CheckConvectionFanStatus();

        if (_nextTemperatureDisplayUpdate < millis()) {
            g_displayQueue.AddItemToQueue(DISPLAY_COMMAND_UPDATE_VALUE, DISPLAY_UPDATE_KEY_PRIMARY_TEMPERATURE, String(_temperaturePrimary));
            g_displayQueue.AddItemToQueue(DISPLAY_COMMAND_UPDATE_VALUE, DISPLAY_UPDATE_KEY_SECONDARY_TEMPERATURE, String(_temperatureSecondary));
            _nextTemperatureDisplayUpdate = millis() + TEMPERATURE_DISPLAY_REFRESH_RATE;
        }

        vTaskDelay(1);
    }
}