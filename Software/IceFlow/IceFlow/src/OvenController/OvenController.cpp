#include "OvenController.h"
#include "../Utilities/PreferencesManager.h"
#include "../DisplayManager/Utilities/DisplayQueue.h"
#include "../Screens/Utilities/ScreenUpdateKeys.h"
#include "PidData.h"

bool OvenController::Init()
{
    pinMode(SSR_OVEN_HEATERS, OUTPUT);
    digitalWrite(SSR_OVEN_HEATERS, RELAY_OFF);

    pinMode(RELAY_CONVECTION_FAN, OUTPUT);
    digitalWrite(RELAY_CONVECTION_FAN, RELAY_OFF);

    //pinMode(STOP_BUTTON, INPUT_PULLUP);

    _autoTune = nullptr;
    _reflow = new Reflow();
    UpdateDoNotExceedTemperature();

    Serial.println("Initializing sensor A...");
    _primaryTemperatureSensor.begin(THERMOCOUPLER_CLK, THERMOCOUPLER_PRIMARY_CS, THERMOCOUPLER_DO);
    _primaryTemperatureSensor.setSWSPIdelay(4);

    Serial.println("Initializing sensor B...");
    _secondaryTemperatureSensor.begin(THERMOCOUPLER_CLK, THERMOCOUPLER_SECONDARY_CS, THERMOCOUPLER_DO);
    _secondaryTemperatureSensor.setSWSPIdelay(4);

    FetchTemperatures();

    _kp = GetPidKP();
    _ki = GetPidKI();
    _kd = GetPidKD();
    _pidController = new PID_v2(_kp, _ki, _kd, PID::Direct);
    _pidController->SetSampleTime(20);

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

void OvenController::ResetPIDs()
{
    _kp = GetPidKP();
    _ki = GetPidKI();
    _kd = GetPidKD();
    _pidController->SetTunings(_kp, _ki, _kd);

}

void OvenController::CheckConvectionFanStatus()
{
    if (_ovenStatus == OS_IDLE && !_convectionFanOn && _temperaturePrimary < MINIUM_OVEN_TEMPERATURE_FOR_FAN_ON) {
        return;
    }

    if (_ovenStatus != OS_IDLE && _convectionFanOn) {
        return;
    }

    if (_ovenStatus != OS_IDLE && !_convectionFanOn) {
        EnableConvectionFan();
        return;
    }

    if(_ovenStatus == OS_IDLE && !_convectionFanOn && _temperaturePrimary > MINIUM_OVEN_TEMPERATURE_FOR_FAN_ON){
        EnableConvectionFan();
        return;
    }

    if (_ovenStatus == OS_IDLE && _convectionFanOn && _temperaturePrimary < MINIUM_OVEN_TEMPERATURE_FOR_FAN_OFF) {
        DisableConvectionFan();
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
    _ovenStatus = OS_IDLE;

    DisableOvenHeaters();
    DisableConvectionFan();
    SetTargetTemperature(0);
    DeleteAutoTune();
    _reflow->Stop();
    DisplayQueue.QueueKey(suk_Oven_Stopped);
    
}

void OvenController::EmergencyStopOven()
{
    StopOven();
    DisplayQueue.QueueKey(suk_Emergency_Oven_Stopped);
}

void OvenController::FetchPrimaryTemperature()
{
    int ret = _primaryTemperatureSensor.read();
    int errorCount = 0;
    while (ret != STATUS_OK) {
        errorCount++;
        if (errorCount >= 3) {
            Serial.println("Failed to get valid temperature from sensor A.");
            return;
        }
        Serial.print("error:\t\t");
        if (_primaryTemperatureSensor.shortToGND())   Serial.println("SHORT TO GROUND");
        if (_primaryTemperatureSensor.shortToVCC())   Serial.println("SHORT TO VCC");
        if (_primaryTemperatureSensor.openCircuit())  Serial.println("OPEN CIRCUIT");
        if (_primaryTemperatureSensor.genericError()) Serial.println("GENERIC ERROR");
        if (_primaryTemperatureSensor.noRead())       Serial.println("NO READ");
        if (_primaryTemperatureSensor.noCommunication()) Serial.println("NO COMMUNICATION");
        delay(2);
        ret = _primaryTemperatureSensor.read();
    }
    _temperaturePrimary = _primaryTemperatureSensor.getTemperature();
}

void OvenController::FetchSecondaryTemperature()
{
    int ret = _secondaryTemperatureSensor.read();
    int errorCount = 0;
    while (ret != STATUS_OK) {
        errorCount++;
        if (errorCount >= 3) {
            Serial.println("Failed to get valid temperature from sensor B.");
            return;
        }
        Serial.print("error:\t\t");
        if (_secondaryTemperatureSensor.shortToGND())   Serial.println("SHORT TO GROUND");
        if (_secondaryTemperatureSensor.shortToVCC())   Serial.println("SHORT TO VCC");
        if (_secondaryTemperatureSensor.openCircuit())  Serial.println("OPEN CIRCUIT");
        if (_secondaryTemperatureSensor.genericError()) Serial.println("GENERIC ERROR");
        if (_secondaryTemperatureSensor.noRead())       Serial.println("NO READ");
        if (_secondaryTemperatureSensor.noCommunication()) Serial.println("NO COMMUNICATION");
        delay(2);
        ret = _secondaryTemperatureSensor.read();
    }
    _temperatureSecondary = _secondaryTemperatureSensor.getTemperature();
}

void OvenController::FetchTemperatures()
{
    if (_nextTemperatureUpdate < millis()) {
        _nextTemperatureUpdate = millis() + OVEN_TEMPERATURE_UPDATE_RATE;

        FetchPrimaryTemperature();
        FetchSecondaryTemperature();

        if (_temperaturePrimary >= _doNotExceedTemperature) {
            EmergencyStopOven();
            delay(100);
            DisplayQueue.QueueKey(suk_Oven_Exceeded_Max);
        }
        if (_streamTemperatures && _nextTemperatureDisplayUpdate < millis()) {
            SendPrimaryTemperatureToDisplay();
            SendSecondaryTemperatureToDisplay();
            _nextTemperatureDisplayUpdate = millis() + TEMPERATURE_DISPLAY_REFRESH_RATE;
        }
    }
}

void OvenController::HandleOvenHeatersWithPID()
{
    const float output = _pidController->Run(_temperaturePrimary);
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
    if (_temperaturePrimary > MINIUM_OVEN_TEMPERATURE_FOR_FAN_ON) {
        Serial.print("Oven must be below: ");
        Serial.print(MINIUM_OVEN_TEMPERATURE_FOR_FAN_ON);
        Serial.println(" degrees Celcius before Reflow can begin");
        return false;
    }

    _ovenStatus = OS_REFLOW_ACTIVE;
    DisplayQueue.QueueKey(suk_Oven_Reflow_On);
    if (!_reflow->Start(_temperaturePrimary)) {
        return false;
    }
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
        if (_reflow->GetReflowStage() != RS_COOLING)
        {
            HandleOvenHeatersWithPID();
        }
        else if (_heatersOn) {
            DisableOvenHeaters();
        }        
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
    PidData pidData;
    if (pidData.GetData()) {
        Serial.println("\n\nCurrent PID Data: ");
        Serial.println(pidData.ToJsonString());
        Serial.println("\n");
    }
    else {
        Serial.println("\n\nNo current PID Data found\n");
    }

    Serial.println("Starting PID Auto Tune!");
    if (_ovenStatus != OS_IDLE && _ovenStatus != OS_MANUAL_HEAT_ACTIVE) {
        Serial.print("Oven must be idle or in manaul mode to start Auto Tune.\nCurrent State: ");
        Serial.println(_ovenStatus);
        return;
    }

    StopOven();

    _ovenStatus = OS_AUTOTUNE_ACTIVE;
    _autoTune = new AutoTune(targetTemperature);
    EnableOvenHeaters();

    char val[7];
    snprintf(val, 7, "%f", targetTemperature);
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

                Serial.printf("\nmaxTemp: %f  minTemp: %f\n", _autoTune->maxTemp, _autoTune->minTemp);
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


        Serial.println("\n\n**AutoTune: FINSIHED!**");
        PidData pidData;
        if (pidData.GetData()) {
            Serial.println("Current PID Data: ");
            Serial.println(pidData.ToJsonString());
            Serial.println("");
        }
        else {
            Serial.println("No current PID Data found");
        }

        
        pidData.midKp = _autoTune->Kp;
        pidData.midKi = _autoTune->Ki;
        pidData.midKd = _autoTune->Kd;
        Serial.printf("Kp: %f  Ki: %f  Kd %f\n", _autoTune->Kp, _autoTune->Ki, _autoTune->Kd);


        _autoTune->Kp = 0.33* _autoTune->Ku;
        _autoTune->Ki = _autoTune->Kp/ _autoTune->Tu;
        _autoTune->Kd = _autoTune->Kp* _autoTune->Tu/3;
        pidData.lowKp = _autoTune->Kp;
        pidData.lowKi = _autoTune->Ki;
        pidData.lowKd = _autoTune->Kd;
        Serial.printf("Some overshoot:   Kp: %f  Ki: %f  Kd %f\n", _autoTune->Kp, _autoTune->Ki, _autoTune->Kd);


        _autoTune->Kp = 0.2* _autoTune->Ku;
        _autoTune->Ki = 2* _autoTune->Kp/ _autoTune->Tu;
        _autoTune->Kd = _autoTune->Kp* _autoTune->Tu/3;
        pidData.highKp = _autoTune->Kp;
        pidData.highKi = _autoTune->Ki;
        pidData.highKd = _autoTune->Kd;
        Serial.printf("No overshoot:   Kp: %f  Ki: %f  Kd %f\n", _autoTune->Kp, _autoTune->Ki, _autoTune->Kd);

        Serial.println("ALL VALUES ARE EXPERIMENTAL.\nNO WARRANTIES OR GUARANTEES ARE MADE AT ALL.\n\n****DO YOU HAVE A FIRE EXTINGUISHER HANDY?****\n\n");
        StopOven();

        Serial.println("\nJSON:");
        Serial.println(pidData.ToJsonString());
        if (!pidData.SaveData()) {
            Serial.println("Failed to save PID Data to file.");
        }

        DisplayQueue.QueueKey(suk_Oven_AutoTune_Complete);
        return;
    }
}

void OvenController::Run()
{
    while (true) {
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
    char val[7];
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
        snprintf(val, 7, "%f", _autoTune->targetTemperature);
        DisplayQueue.QueueKeyAndValue(suk_Oven_AutoTune_On, val);
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

void OvenController::UpdateDoNotExceedTemperature()
{
    float x = (float)GetOvenDoNotExceedTemperature();
    Serial.print("Temp:");
    Serial.println(x);
    _doNotExceedTemperature = (float)GetOvenDoNotExceedTemperature();
}

OvenController OvenManager;