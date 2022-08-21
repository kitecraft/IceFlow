#pragma once
#include <Arduino.h>
#include <PID_v2.h>
#include "../IceFlow_Config.h"
#include "OvenEnums.h"
#include "../ProfileManager/ProfileManager.h"
#include "../DisplayManager/Utilities/DisplayQueue.h"
#include "../Screens/Utilities/ScreenUpdateKeys.h"
#include "AutoTune.h"
#include "Reflow.h"
#include "MAX31855.h"

class OvenController
{
private:
	TaskHandle_t _taskHandle = nullptr;
	Reflow* _reflow = nullptr;
	OvenStatus _ovenStatus = OS_IDLE;

	unsigned long _lastButtonPress = 0;

	bool _heatersOn = false;
	bool _convectionFanOn = false;
	int _targetTemperature = 0;

	MAX31855 _primaryTemperatureSensor;
	unsigned long _nextTemperatureUpdate = 0;

	unsigned long _nextTemperatureDisplayUpdate = 0;
	double _temperaturePrimary = 15;
	double _temperatureSecondary = 5;
	bool _streamTemperatures = false;

	//bool _testDirection = true;

	//PID
	AutoTune* _autoTune = nullptr;
	PID_v2* _pidController = nullptr;
	double _kp;
	double _ki;
	double _kd;
	void DeleteAutoTune();

	Profile _profile;

	void CheckConvectionFanStatus();
	void EnableConvectionFan();
	void DisableConvectionFan();
	void EnableOvenHeaters();
	void DisableOvenHeaters();

	void FetchPrimaryTemperature();
	void FetchSecondaryTemperature();
	void FetchTemperatures();

	void SetTargetTemperature(float target);
	void HandleOvenHeatersWithPID();
	void HandleReflowSession();
	void HandleAutoTune();

	void SendPrimaryTemperatureToDisplay();
	void SendSecondaryTemperatureToDisplay();
	void SendTargetTemperatureToDisplay();

public:
	bool Init();
	void Run();

	void StopOven();
	void EmergencyStopOven();

	bool StartReflowSession();
	void StartManualHeat(int targetTemperature);
	void StartTemperatureStream() { _streamTemperatures = true; DisplayQueue.QueueKey(suk_TemperatureStreamStarted); }
	void StopTemperatureStream() { _streamTemperatures = false; }

	void StartAutoTune(float targetTemperature);
	void SendStatus();
};

extern OvenController OvenManager;
void static IRAM_ATTR OvenControllerTask(void*)
{
	OvenManager.Run();
}