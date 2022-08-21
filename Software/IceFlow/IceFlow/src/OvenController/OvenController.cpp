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

    _autoTune = nullptr;
    _reflow = new Reflow();

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
    if (_ovenStatus == OS_IDLE && !_convectionFanOn && _temperaturePrimary < MINIUM_OVEN_TEMPERATURE_FOR_FAN) {
        return;
    }

    if (_ovenStatus != OS_IDLE && _convectionFanOn) {
        return;
    }

    if (_ovenStatus != OS_IDLE && !_convectionFanOn) {
        EnableConvectionFan();
        return;
    }

    if(_ovenStatus == OS_IDLE && !_convectionFanOn && _temperaturePrimary > MINIUM_OVEN_TEMPERATURE_FOR_FAN){
        EnableConvectionFan();
        return;
    }

    if (_ovenStatus == OS_IDLE && _convectionFanOn && _temperaturePrimary < MINIUM_OVEN_TEMPERATURE_FOR_FAN) {
        DisableConvectionFan();
        return;
    }

    /*
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
    */
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
    _ovenStatus = OS_IDLE;
    _reflowPhase = RP_NOT_ACTIVE;

    DisableOvenHeaters();
    DisableConvectionFan();
    SetTargetTemperature(0);
    DeleteAutoTune();
    DisplayQueue.QueueKey(suk_Oven_Stopped);
    
}

void OvenController::EmergencyStopOven()
{
    StopOven();
    DisplayQueue.QueueKey(suk_Emergency_Oven_Stopped);
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
    _temperatureSecondary = _temperaturePrimary - 10;
}

void OvenController::FetchTemperatures()
{
    //FetchPrimaryTemperature();
    //FetchSecondaryTemperature();

    if (_heatersOn) {
        _temperaturePrimary += 0.001;
        _temperatureSecondary += 0.001;
    }
    else {
        _temperaturePrimary -= 0.001;
        _temperatureSecondary -= 0.001;

        if (_temperaturePrimary < 15) {
            _temperaturePrimary = 15;
        }
        if (_temperatureSecondary < 5) {
            _temperatureSecondary = 5;
        }
    }

    if (_streamTemperatures && _nextTemperatureDisplayUpdate < millis()) {
        SendPrimaryTemperatureToDisplay();
        SendSecondaryTemperatureToDisplay();
        _nextTemperatureDisplayUpdate = millis() + TEMPERATURE_DISPLAY_REFRESH_RATE;


        /*
        char val[7];
        snprintf(val, 7, "%.2f", _temperaturePrimary);
        DisplayQueue.QueueKeyAndValue(suk_PrimaryTemperature, val);

        //snprintf(val, 7, "%.2f", _temperatureSecondary);
        snprintf(val, 7, "%.2f", _temperaturePrimary - 5);
        DisplayQueue.QueueKeyAndValue(suk_SecondaryTemperature, val);
        _nextTemperatureDisplayUpdate = millis() + TEMPERATURE_DISPLAY_REFRESH_RATE;
        */
    }

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

void OvenController::SetTargetTemperature(float target)
{
    _targetTemperature = target;
    _pidController->Setpoint(_targetTemperature);
}

void OvenController::StartManualHeat(int targetTemperature)
{
    if (_ovenStatus == OS_REFLOW_ACTIVE) {
        return;
    }
    _ovenStatus = OS_MANUAL_HEAT_ACTIVE;
    SetTargetTemperature(targetTemperature);

    char val[4];
    snprintf(val, 4, "%i", _targetTemperature);
    DisplayQueue.QueueKeyAndValue(suk_Oven_Manual_On, val);
}

bool OvenController::StartReflowSession()
{
    if (_temperaturePrimary > MINIUM_OVEN_TEMPERATURE_FOR_FAN) {
        Serial.print("Oven must be below: ");
        Serial.print(MINIUM_OVEN_TEMPERATURE_FOR_FAN);
        Serial.println(" degrees Celcius before Reflow can begin");
        return false;
    }

    _ovenStatus = OS_REFLOW_ACTIVE;
    DisplayQueue.QueueKey(suk_Oven_Reflow_On);
    _reflow->Start(_temperaturePrimary);
    int startT = _reflow->GetStartingTemperature();
    SetTargetTemperature(startT);
    SendTargetTemperatureToDisplay();
    return true;
}

void OvenController::HandleReflowSession()
{
    int newTarget;
    ReflowProcessReturn ret = _reflow->Process(_temperaturePrimary, newTarget);

    switch (ret) {
    case RPR_COMPLETE:
        StopOven();
        break;
    case RPR_ERROR:
        Serial.print("HandleReflowSession Exiting here:");
        StopOven();
        break;
    case RPR_OK:
        if (newTarget != _targetTemperature) {
            SetTargetTemperature(newTarget);
            SendTargetTemperatureToDisplay();
        }
        HandleOvenHeatersWithPID();
        break;
    }
}

void OvenController::SendPrimaryTemperatureToDisplay()
{
    char val[7];
    snprintf(val, 7, "%.2f", _temperaturePrimary);
    DisplayQueue.QueueKeyAndValue(suk_PrimaryTemperature, val);
}

void OvenController::SendSecondaryTemperatureToDisplay()
{
    char val[7];
    snprintf(val, 7, "%.2f", _temperatureSecondary);
    DisplayQueue.QueueKeyAndValue(suk_SecondaryTemperature, val);
}

void OvenController::SendTargetTemperatureToDisplay()
{
    char val[4];
    snprintf(val, 4, "%i", _targetTemperature);
    DisplayQueue.QueueKeyAndValue(suk_TertiaryTemperature, val);
}

void OvenController::StartAutoTune(float targetTemperature)
{
    Serial.println("Starting PID Auto Tune!");
    if (_ovenStatus != OS_IDLE && _ovenStatus != OS_MANUAL_HEAT_ACTIVE) {
        Serial.print("Oven must be idle or in mnaul mode to start Auto Tune.\nCurrent State: ");
        Serial.println(_ovenStatus);
        return;
    }

    StopOven();

    _ovenStatus = OS_AUTOTUNE_ACTIVE;
    _autoTune = new AutoTune(targetTemperature);
    EnableOvenHeaters();

    char val[4];
    snprintf(val, 4, "%f", targetTemperature);
    DisplayQueue.QueueKeyAndValue(suk_Oven_AutoTune_On, val);
}

void OvenController::DeleteAutoTune()
{
    if (_autoTune != nullptr) {
        delete _autoTune;
        _autoTune = nullptr;
    }
}

void OvenController::HandleAutoTune()
{
    _autoTune->currentTemp = _temperaturePrimary;
    unsigned long time = millis();
    _autoTune->maxTemp = max(_autoTune->maxTemp, _autoTune->currentTemp);
    _autoTune->minTemp = min(_autoTune->minTemp, _autoTune->currentTemp);
    if (_autoTune->heating == true && _autoTune->currentTemp > _autoTune->targetTemperature)   // switch heating -> off
    {
        if (time - _autoTune->t2 > 2500)
        {
            _autoTune->heating = false;
            DisableOvenHeaters();

            _autoTune->t1 = time;
            _autoTune->t_high = _autoTune->t1 - _autoTune->t2;
            _autoTune->maxTemp = _temperaturePrimary;
        }
    }
    if (_autoTune->heating == false && _autoTune->currentTemp < _autoTune->targetTemperature)
    {
        if (time - _autoTune->t1 > 5000)
        {
            _autoTune->heating = true;
            _autoTune->t2 = time;
            _autoTune->t_low = _autoTune->t2 - _autoTune->t1; // half wave length
            if (_autoTune->cycles > 0)
            {

                Serial.printf("maxTemp: %f  minTemp: %f\n", _autoTune->maxTemp, _autoTune->minTemp);
                if (_autoTune->cycles > 2)
                {
                    // Parameter according Ziegler¡§CNichols method: http://en.wikipedia.org/wiki/Ziegler%E2%80%93Nichols_method
                    _autoTune->Ku = (4.0 * _autoTune->d) / (3.14159 * (_autoTune->maxTemp - _autoTune->minTemp));
                    _autoTune->Tu = ((float)(_autoTune->t_low + _autoTune->t_high) / 1000.0);
                    Serial.printf("Ku: %f  Tu: %f\n", _autoTune->Ku, _autoTune->Tu);
                    _autoTune->Kp = 0.6 * _autoTune->Ku;
                    _autoTune->Ki = 2 * _autoTune->Kp / _autoTune->Tu;
                    _autoTune->Kd = _autoTune->Kp * _autoTune->Tu * 0.125;
                    Serial.printf("Kp: %f  Ki: %f  Kd %f\n", _autoTune->Kp, _autoTune->Ki, _autoTune->Kd);
                }
            }

            EnableOvenHeaters();
            _autoTune->cycles++;
            _autoTune->minTemp = _temperaturePrimary;
        }
    }
    if (_autoTune->currentTemp > (_temperaturePrimary + 20))
    {
        StopOven();
        DeleteAutoTune();
        Serial.println("AutoTune: Excceded Max Temperature!!  Bailing out");
        return;
    }
    if (time - _autoTune->temp_millis > 1000)
    {
        _autoTune->temp_millis = time;
        // Print temperatures here because all is well
    }
    if (((time - _autoTune->t1) + (time - _autoTune->t2)) > (10L * 60L * 1000L * 2L))   // 20 Minutes
    {
        StopOven();
        DeleteAutoTune();
        Serial.println("AutoTune: Exceeded run time limit!!  Bailing out");
        return;
    }
    if (_autoTune->cycles > 5)
    {
        DisableOvenHeaters();
        Serial.println("AutoTune: FINSIHED!");
        Serial.printf("Kp: %f  Ki: %f  Kd %f\n", _autoTune->Kp, _autoTune->Ki, _autoTune->Kd);

        _autoTune->Kp = 0.33* _autoTune->Ku;
        _autoTune->Ki = _autoTune->Kp/ _autoTune->Tu;
        _autoTune->Kd = _autoTune->Kp* _autoTune->Tu/3;
        Serial.printf("Some overshoot:   Kp: %f  Ki: %f  Kd %f\n", _autoTune->Kp, _autoTune->Ki, _autoTune->Kd);


        _autoTune->Kp = 0.2* _autoTune->Ku;
        _autoTune->Ki = 2* _autoTune->Kp/ _autoTune->Tu;
        _autoTune->Kd = _autoTune->Kp* _autoTune->Tu/3;
        Serial.printf("No overshoot:   Kp: %f  Ki: %f  Kd %f\n", _autoTune->Kp, _autoTune->Ki, _autoTune->Kd);

        Serial.println("ALL VALUES ARE EXPERIMENTAL.\nNO WARRANTIES OR GUARANTEES ARE MADE AT ALL.\n\n****DO YOU HAVE A FIRE EXTINGUISHER HANDY?****\n\n");
        StopOven();
        return;
    }
}

void OvenController::Run()
{
    while (true) {

        if (digitalRead(STOP_BUTTON) == LOW) {
            if (millis() - STOP_BUTTON_DEBOUNCE_TIME > _lastButtonPress) {
                EmergencyStopOven();
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
        case OS_AUTOTUNE_ACTIVE:
            HandleAutoTune();
            break;
        default:
            StopOven();
            Serial.println("\n***\nSomething has gone horribly wrong.\nPlease unplug the oven.\nThe warranty is void\n***\n");
            break;
        }

        CheckConvectionFanStatus();
        
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
        snprintf(val, 4, "%i", _targetTemperature);
        DisplayQueue.QueueKeyAndValue(suk_Oven_Manual_On, val);
        break;
    case OS_REFLOW_ACTIVE:
        DisplayQueue.QueueKey(suk_Oven_Reflow_On);
        break;
    case OS_AUTOTUNE_ACTIVE:
        DisplayQueue.QueueKey(suk_Oven_AutoTune_On);
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