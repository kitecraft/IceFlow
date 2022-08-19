#include "Reflow.h"
#include "../DisplayManager/Utilities/DisplayQueue.h"
#include "../Screens/Utilities/ScreenUpdateKeys.h"

Reflow::Reflow()
{
	Stop();
}

Reflow::~Reflow()
{

}

bool Reflow::Start()
{
	if (!ProfileManager.GetSavedProfile(&_profile)) {
		Serial.println("Failed to load Profile");
	}

	_degreesPerSecond = ((float)_profile.pre_heat_target_temperature - 50.0) / (float)_profile.pre_heat_ramp_time;

	_stage = RS_PRE_PREHEAT;
	_currentTemperature = 0;
	_targetTemperature = 50;
	_startTime = millis();
	_stageStartTime = _startTime;
	_lastTemperatureChange = _startTime;
	return true;
}

bool Reflow::Stop()
{
	_stage = RS_NOT_ACTIVE;
	_active = false;
	return true;
}

ReflowProcessReturn Reflow::Process(float currentTemperature, int& targetTemperature)
{
	_currentTemperature = currentTemperature;

	switch (_stage) {
	case RS_PRE_PREHEAT:
		ProcessPrePreHeat();
		break;
	case RS_PREHEAT:
		ProcessPreHeat();
		break;
	case RS_SOAK:
		ProcessSoak();
		break;
	case RS_RAMP:
		break;
	case RS_REFLOW:
		break;
	case RS_COOLING:
		break;

	default:
		break;
	}

	targetTemperature = _targetTemperature;

	return _processReturn;
}

void Reflow::ProcessPrePreHeat()
{
	_processReturn = RPR_OK;

	if (_currentTemperature >= 50) {
		_stage = RS_PREHEAT;
		_stageStartTime = millis();
		ProcessPreHeat();
	}
}

void Reflow::ProcessPreHeat()
{
	_processReturn = RPR_OK;
	if (_currentTemperature >= _profile.pre_heat_target_temperature) {
		Serial.println("STARTING SOAK!");
		_stageRunTimes[RS_PREHEAT] = millis() - _startTime;
		DisplayQueue.QueueKey(suk_Reflow_StageComplete_PreHeat);

		char val[4];
		snprintf(val, 4, "%i", _stageRunTimes[RS_PREHEAT]/1000);
		DisplayQueue.QueueKeyAndValue(suk_Reflow_PreHeat_Runtime, val);

		_stage = RS_SOAK;
		_stageStartTime = millis();
		ProcessSoak();
		return;
	}
	
	if (_targetTemperature >= _profile.pre_heat_target_temperature ||
		millis() - _lastTemperatureChange < 1000) {
		return;
	}

	_targetTemperature = round(50.0 + (((float)(millis() - _stageStartTime)/1000.0) * _degreesPerSecond));

	if (_targetTemperature > _profile.pre_heat_target_temperature) {
		_targetTemperature = _profile.pre_heat_target_temperature;
	}
	_lastTemperatureChange = millis();
}

void Reflow::ProcessSoak()
{
	_processReturn = RPR_OK;

}