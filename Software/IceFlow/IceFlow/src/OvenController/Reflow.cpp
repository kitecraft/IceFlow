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

void Reflow::Init()
{
	_stage = RS_NOT_ACTIVE;
	_stageStartTime = 0;
	_reflowActivityStartTime = 0;
	_startTemperature;
	_currentTemperature = 0;
	_targetTemperature = 0;

	_stageRunTimes[0] = 0;
	_stageRunTimes[1] = 0;
	_stageRunTimes[2] = 0;
	_stageRunTimes[3] = 0;
	_stageRunTimes[4] = 0;
	_stageCountDown = 0;
	_preHeatStageTime = 0;
	_lastTemperatureChange = 0;
	_degreesPerSecond = 0;

	_processReturn = RPR_OK;
}

bool Reflow::Start(int currentTemperature)
{
	Init();

	if (!ProfileManager.GetSavedProfile(&_profile)) {
		Serial.println("Failed to load Profile");
	}


	_stage = RS_PREHEAT;
	_currentTemperature = currentTemperature;
	_startTemperature = (_currentTemperature < REFLOW_MIN_TEMPERATURE) ? REFLOW_MIN_TEMPERATURE : _currentTemperature;
	_degreesPerSecond = (float)(_profile.pre_heat_target_temperature - _startTemperature) / (float)_profile.pre_heat_ramp_time;
	_targetTemperature = _startTemperature;

	_reflowActivityStartTime = millis();
	_stageStartTime = millis();
	_preHeatStageTime = 0;
	_lastTemperatureChange = 0;
	return true;
}

bool Reflow::Stop()
{
	_stage = RS_NOT_ACTIVE;
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
		_stageRunTimes[RS_PREHEAT] = millis() - _stageStartTime;
		char val[7];
		snprintf(val, 7, "%f", _stageRunTimes[RS_PREHEAT] / 1000.0);
		DisplayQueue.QueueKeyAndValue(suk_Reflow_StageComplete_PreHeat, val);

		_stage = RS_SOAK;
		_stageCountDown = _profile.pre_heat_soak_time;
		_degreesPerSecond = (float)(_profile.pre_heat_soak_end_temperature - _profile.pre_heat_target_temperature) / (float)_stageCountDown;
		_preHeatStageTime = millis();
		_stageStartTime = millis();
		_lastTemperatureChange = millis();

		ProcessSoak();
		return;
	}
	
	if (_targetTemperature >= _profile.pre_heat_target_temperature ||
		_currentTemperature < _startTemperature ||
		millis() - _lastTemperatureChange < 1000) {
		return;
	}

	if (_preHeatStageTime == 0) {
		_preHeatStageTime = millis();
	}
	
	 _targetTemperature = round((((float)(millis() - _preHeatStageTime) / 1000.0) * _degreesPerSecond) + _startTemperature);
	if (_targetTemperature > _profile.pre_heat_target_temperature) {
		_targetTemperature = _profile.pre_heat_target_temperature;
	}
	_lastTemperatureChange = millis();
}

void Reflow::ProcessSoak()
{
	_processReturn = RPR_OK;
	if (_stageCountDown <= 0 && _currentTemperature >= _profile.pre_heat_soak_end_temperature) {
		_stageRunTimes[RS_SOAK] = millis() - _stageStartTime;
		char val[7];
		snprintf(val, 7, "%f", _stageRunTimes[RS_SOAK] / 1000.0);
		DisplayQueue.QueueKeyAndValue(suk_Reflow_StageComplete_Soak, val);

		_stage = RS_RAMP;
		_degreesPerSecond = (float)(_profile.reflow_target_temperature - _profile.pre_heat_soak_end_temperature) / (float)_profile.reflow_ramp_time;
		_stageStartTime = millis();
		_lastTemperatureChange = millis();
		return;
	}


	if (millis() - _lastTemperatureChange < 1000){
		return;
	}
	_lastTemperatureChange = millis();


	_targetTemperature = round(((float)(_profile.pre_heat_soak_time - _stageCountDown) * _degreesPerSecond) + (float)_profile.pre_heat_target_temperature);
	if (_targetTemperature > _profile.pre_heat_soak_end_temperature) {
		_targetTemperature = _profile.pre_heat_soak_end_temperature;
	}

	_stageCountDown--;
	if (_stageCountDown < 0) {
		_stageCountDown = 0;
	}
}

void Reflow::ProcessRamp()
{
	_processReturn = RPR_OK;
	if (_currentTemperature >= _profile.reflow_target_temperature) {
		_stageRunTimes[RS_RAMP] = millis() - _stageStartTime;
		char val[7];
		snprintf(val, 7, "%f", _stageRunTimes[RS_RAMP] / 1000.0);
		DisplayQueue.QueueKeyAndValue(suk_Reflow_StageComplete_Ramp,val);

		_stage = RS_REFLOW;

		_stageCountDown = _profile.reflow_soak_time;
		_degreesPerSecond = 0;
		_targetTemperature = _profile.reflow_target_temperature;

		_stageStartTime = millis();
		_lastTemperatureChange = millis();

		return;
	}

	if (_targetTemperature >= _profile.reflow_target_temperature ||
		millis() - _lastTemperatureChange < 1000) {
		return;
	}

	_targetTemperature = round((((float)(millis() - _stageStartTime) / 1000.0) * _degreesPerSecond) + _profile.pre_heat_soak_end_temperature);
	if (_targetTemperature > _profile.reflow_target_temperature) {
		_targetTemperature = _profile.reflow_target_temperature;
	}
	_lastTemperatureChange = millis();
}

void Reflow::ProcessReflow()
{
	_processReturn = RPR_OK;
	if (_stageCountDown <= 0) {
		_stageRunTimes[RS_REFLOW] = millis() - _stageStartTime;
		char val[7];
		snprintf(val, 7, "%f", _stageRunTimes[RS_REFLOW] / 1000.0);
		DisplayQueue.QueueKeyAndValue(suk_Reflow_StageComplete_Reflow,val);

		_stage = RS_COOLING;
		_degreesPerSecond = _profile.cooling_ramp_down_speed;
		_stageStartTime = millis();
		_lastTemperatureChange = 0;
		_startTemperature = _currentTemperature;
		return;
	}

	if (millis() - _lastTemperatureChange < 1000) {
		return;
	}
	_stageCountDown--;
	_lastTemperatureChange = millis();
}

void Reflow::ProcessCooling()
{
	_processReturn = RPR_OK;
	if (_currentTemperature <= 50) {
		_stageRunTimes[RS_COOLING] = millis() - _stageStartTime;
		char val[7];
		snprintf(val, 7, "%f", _stageRunTimes[RS_COOLING] / 1000.0);
		DisplayQueue.QueueKeyAndValue(suk_Reflow_StageComplete_Cooling, val);
		
		snprintf(val, 7, "%f", (millis() - _reflowActivityStartTime) / 1000.0);
		DisplayQueue.QueueKeyAndValue(suk_Reflow_Complete, val);

		_targetTemperature = 10;
		_stage = RS_COMPLETE;
		_processReturn = RPR_COMPLETE;
	}

	if (millis() - _lastTemperatureChange < 1000) {
		return;
	}
	_targetTemperature = _startTemperature - round((((float)(millis() - _stageStartTime) / 1000.0) * _degreesPerSecond));
	if (_targetTemperature < 50) {
		_targetTemperature = 20;
	}
	_lastTemperatureChange = millis();
}