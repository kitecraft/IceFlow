#pragma once
#include <Arduino.h>
#include "ReflowStage.h"
#include "../ProfileManager/ProfileManager.h"

#define REFLOW_MIN_TEMPERATURE 30

enum ReflowProcessReturn {
	RPR_OK = 0,
	RPR_ERROR
};

class Reflow
{
private:
	Profile _profile;
	bool _active = false;
	REFLOW_STAGE _stage = RS_NOT_ACTIVE;
	unsigned long _startTime = 0;
	int _startTemperature;
	int _currentTemperature = 0;
	int _targetTemperature = 0;

	unsigned long _stageRunTimes[5];
	unsigned long _stageTime = 0;
	unsigned long _lastTemperatureChange = 0;
	float _degreesPerSecond = 0;

	ReflowProcessReturn _processReturn = RPR_OK;
	void ProcessPreHeat();
	void ProcessSoak();
	void ProcessRamp();
	void ProcessReflow();
	void ProcessCooling();
public:
	Reflow();
	~Reflow();

	bool IsActive() { return _active; }
	bool Start(int currentTemperature);
	bool Stop();
	ReflowProcessReturn Process(float currentTemperature, int& targetTemperature);
	int GetStartingTemperature() { return _startTemperature; }
};

