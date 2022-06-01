#include "OvenController.h"

bool OvenController::Init()
{
    pinMode(RELAY_TOP_PIN, OUTPUT);
    digitalWrite(RELAY_TOP_PIN, RELAY_OFF);

    pinMode(RELAY_BOTTOM_PIN, OUTPUT);
    digitalWrite(RELAY_BOTTOM_PIN, RELAY_OFF);

    pinMode(RELAY_CONVECTION_FAN, OUTPUT);
    digitalWrite(RELAY_CONVECTION_FAN, RELAY_OFF);

    return true;
}

void OvenController::CheckConvectionFanStatus()
{
    //If oven is On, and manual mode is on, and Fan is on, 
    //Turn of manual mode when temperature exceeds minium
    if (_ovenOn && _convectionFanOnManual && (_temperaturePrimary > MINIUM_OVEN_TEMPERATURE_FOR_FAN)) {
        EnableConvectionFan(); //Make sure it's on
        _convectionFanOnManual = false;
        return;
    }
    //If the oven and fan are both on, return
    else if (_ovenOn && _convectionFanOn) {
        return;
    }
    //If the oven is on, but the fan isn't, turn the fan on
    else if (_ovenOn && !_convectionFanOn) {
        EnableConvectionFan();
        _convectionFanOnManual = false;
        _convectionFanOnManual = false;
    }
    //If the oven is off, and the fan is on, turn if off if the temperature is below threshold
    //and not in manual mode
    else if (!_ovenOn && (_convectionFanOn && (_temperaturePrimary < MINIUM_OVEN_TEMPERATURE_FOR_FAN) && !_convectionFanOnManual))
    {
        DisableConvectionFan();
        _convectionFanOnManual = false;
        _convectionFanOn = false;
    }
    else if (!_ovenOn && !_convectionFanOn && (_temperaturePrimary > MINIUM_OVEN_TEMPERATURE_FOR_FAN)) {
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
}

void OvenController::EnableTopBurner()
{
    digitalWrite(RELAY_TOP_PIN, RELAY_ON);
    _topBurnerOn = true;
}

void OvenController::DisableTopBurner()
{
    digitalWrite(RELAY_TOP_PIN, RELAY_OFF);
    _topBurnerOn = false;
}

void OvenController::EnableBottomBurner()
{
    digitalWrite(RELAY_BOTTOM_PIN, RELAY_ON);
    _bottomBurnerOn = true;
}

void OvenController::DisableBottomBurner()
{
    digitalWrite(RELAY_BOTTOM_PIN, RELAY_OFF);
    _bottomBurnerOn = false;
}

//Disable both top and bottom burners
void OvenController::DisableAllBurners() 
{
    DisableTopBurner();
    DisableBottomBurner();
}

void OvenController::CheckOvenOnStatus()
{
    if (_ovenOn && (_bottomBurnerOn || _topBurnerOn)) {
        return;
    }
    else if (!_ovenOn && (!_bottomBurnerOn && !_topBurnerOn)) {
        return;
    }
    else if (!_ovenOn && (_bottomBurnerOn || _topBurnerOn)) {
        _ovenOn = true;
        return;
    }
    else {
        _ovenOn = false;
    }
}

void OvenController::Run()
{
    while (true) {
        CheckOvenOnStatus();
        CheckConvectionFanStatus();

        vTaskDelay(1);
    }
}