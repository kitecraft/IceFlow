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

bool Reflow::Start(int currentTemperature)
{
	if (!ProfileManager.GetSavedProfile(&_profile)) {
		Serial.println("Failed to load Profile");
	}


	_stage = RS_PREHEAT;
	_currentTemperature = currentTemperature;
	_startTemperature = (_currentTemperature < REFLOW_MIN_TEMPERATURE) ? REFLOW_MIN_TEMPERATURE : _currentTemperature;
	_degreesPerSecond = (float)(_profile.pre_heat_target_temperature - _startTemperature) / (float)_profile.pre_heat_ramp_time;
	_targetTemperature = _startTemperature;

	_startTime = millis();
	_stageTime = 0;
	_lastTemperatureChange = 0;
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
	case RS_PREHEAT:
		ProcessPreHeat();
		break;
	case RS_SOAK:
		ProcessSoak();
		break;
	case RS_RAMP:
		ProcessRamp();
		break;
	case RS_REFLOW:
		ProcessReflow();
		break;
	case RS_COOLING:
		ProcessCooling();
		break;
	default:
		break;
	}

	targetTemperature = _targetTemperature;

	return _processReturn;
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


		_stageTime = _profile.pre_heat_soak_time;
		_degreesPerSecond = (float)(_profile.pre_heat_soak_end_temperature - _profile.pre_heat_target_temperature) / (float)_stageTime;
		_targetTemperature = _startTemperature;

		_startTime = millis();
		_lastTemperatureChange = 0;

		ProcessSoak();
		return;
	}
	
	if (_targetTemperature >= _profile.pre_heat_target_temperature ||
		_currentTemperature < _startTemperature ||
		millis() - _lastTemperatureChange < 1000) {
		return;
	}

	if (_stageTime == 0) {
		_stageTime = millis();
	}
	
	 _targetTemperature = round((((float)(millis() - _stageTime) / 1000.0) * _degreesPerSecond) + _startTemperature);
	if (_targetTemperature > _profile.pre_heat_target_temperature) {
		_targetTemperature = _profile.pre_heat_target_temperature;
	}
	_lastTemperatureChange = millis();
}

void Reflow::ProcessSoak()
{
	_processReturn = RPR_OK;
	if (_stageTime <= 0 && _currentTemperature >= _profile.pre_heat_soak_end_temperature) {
		_stageRunTimes[RS_SOAK] = _profile.pre_heat_soak_time;;
		DisplayQueue.QueueKey(suk_Reflow_StageComplete_Soak);

		_stage = RS_RAMP;
		_stageTime = 0;
		_degreesPerSecond = (float)(_profile.reflow_target_temperature - _profile.pre_heat_soak_end_temperature) / (float)_profile.reflow_ramp_time;
		_startTime = millis();
		return;
	}

	if (millis() - _lastTemperatureChange < 1000 ||
		_targetTemperature == _profile.pre_heat_soak_end_temperature) {
		return;
	}
	_stageTime--;

	_targetTemperature = round(((_profile.pre_heat_soak_time - _stageTime) * _degreesPerSecond) + _profile.pre_heat_target_temperature);
	if (_targetTemperature > _profile.pre_heat_soak_end_temperature) {
		_targetTemperature = _profile.pre_heat_soak_end_temperature;
	}
	_lastTemperatureChange = millis();
}

void Reflow::ProcessRamp()
{
	_processReturn = RPR_OK;
	if (_currentTemperature >= _profile.reflow_target_temperature) {
		Serial.println("STARTING REFLOW!");
		_stageRunTimes[RS_RAMP] = millis() - _startTime;
		DisplayQueue.QueueKey(suk_Reflow_StageComplete_Ramp);

		char val[4];
		snprintf(val, 4, "%i", _stageRunTimes[RS_RAMP] / 1000);
		DisplayQueue.QueueKeyAndValue(suk_Reflow_Ramp_Runtime, val);
		_stage = RS_REFLOW;


		_stageTime = _profile.reflow_soak_time;
		_degreesPerSecond = 0;
		_targetTemperature = _profile.reflow_target_temperature;

		_startTime = millis();
		_lastTemperatureChange = 0;


		return;
	}

	if (_targetTemperature >= _profile.reflow_target_temperature ||
		millis() - _lastTemperatureChange < 1000) {
		return;
	}

	_targetTemperature = round((((float)(millis() - _startTime) / 1000.0) * _degreesPerSecond) + _profile.pre_heat_soak_end_temperature);
	if (_targetTemperature > _profile.reflow_target_temperature) {
		_targetTemperature = _profile.reflow_target_temperature;
	}
	_lastTemperatureChange = millis();
}

void Reflow::ProcessReflow()
{
	_processReturn = RPR_OK;
	if (_stageTime <= 0) {
		_stageRunTimes[RS_REFLOW] = _profile.reflow_soak_time;
		DisplayQueue.QueueKey(suk_Reflow_StageComplete_Reflow);

		_stage = RS_COOLING;
		_stageTime = 0;
		_degreesPerSecond = _profile.cooling_ramp_down_speed;
		_startTime = millis();
		_lastTemperatureChange = 0;
		_startTemperature = _currentTemperature;
		return;
	}

	if (millis() - _lastTemperatureChange < 1000) {
		return;
	}
	_stageTime--;
	_lastTemperatureChange = millis();
}

void Reflow::ProcessCooling()
{
	_processReturn = RPR_OK;
	if (_currentTemperature <= 50) {
		_stageRunTimes[RS_COOLING] = _profile.reflow_soak_time;
		DisplayQueue.QueueKey(suk_Reflow_Complete);
		_targetTemperature = 10;

		_stage = RS_COMPLETE;
	}


	if (millis() - _lastTemperatureChange < 1000) {
		return;
	}
	_targetTemperature = _startTemperature - round((((float)(millis() - _startTime) / 1000.0) * _degreesPerSecond));
	if (_targetTemperature < 50) {
		_targetTemperature = 20;
	}
	_lastTemperatureChange = millis();
}