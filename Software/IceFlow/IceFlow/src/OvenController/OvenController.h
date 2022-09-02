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
	float _doNotExceedTemperature = 0;

	MAX31855 _primaryTemperatureSensor;
	MAX31855 _secondaryTemperatureSensor;
	
	unsigned long _nextTemperatureUpdate = 0;
	unsigned long _nextTemperatureDisplayUpdate = 0;
	float _temperaturePrimary = 25;
	float _temperatureSecondary = 25;
	bool _streamTemperatures = false;

	//PID
	AutoTune* _autoTune = nullptr;
	PID_v2* _pidController = nullptr;
	float _kp;
	float _ki;
	float _kd;
	void DeleteAutoTune();

	Profile _profile;

	void CheckConvectionFanStatus();
	void EnableConvectionFan();
	void DisableConvectionFan();
	void EnableOvenHeaters();
	void DisableOvenHeaters();

	void StartPrimaryTemperatureSensor();
	void StartSecondaryTemperatureSensor();
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
	OvenController();
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

	void ResetPIDs();
	void UpdateDoNotExceedTemperature();
};

extern OvenController OvenManager;
void static IRAM_ATTR OvenControllerTask(void*)
{
	OvenManager.Run();
}