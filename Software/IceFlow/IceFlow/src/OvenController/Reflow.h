#pragma once
#include <Arduino.h>
#include "ReflowStage.h"
#include "../ProfileManager/ProfileManager.h"


enum ReflowProcessReturn {
	RPR_OK = 0,
	RPR_COMPLETE,
	RPR_ERROR
};

class Reflow
{
private:
	Profile _profile;
	REFLOW_STAGE _stage = RS_NOT_ACTIVE;
	unsigned long _stageStartTime = 0;
	unsigned long _reflowActivityStartTime = 0;
	int _startTemperature;
	int _currentTemperature = 0;
	int _targetTemperature = 0;

	unsigned long _stageRunTimes[5];
	int _stageCountDown = 0;
	unsigned long _preHeatStageTime = 0;
	unsigned long _lastTemperatureChange = 0;
	float _degreesPerSecond = 0;

	ReflowProcessReturn _processReturn = RPR_OK;

	void ProcessPreHeat();
	void ProcessSoak();
	void ProcessRamp();
	void ProcessReflow();
	void ProcessCooling();

	void Init();
public:
	Reflow();
	~Reflow();

	bool Start(int currentTemperature);
	void Stop();
	ReflowProcessReturn Process(float currentTemperature, int& targetTemperature);
	int GetStartingTemperature() { return _startTemperature; }
	REFLOW_STAGE GetReflowStage() { return _stage; }
};

