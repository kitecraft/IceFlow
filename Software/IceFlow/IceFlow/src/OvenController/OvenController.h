#pragma once
#include <Arduino.h>
#include <PID_v2.h>
#include <Bounce2.h>
#include "../IceFlow_Config.h"
#include "OvenEnums.h"
#include "../ProfileManager/ProfileManager.h"
#include "../DisplayManager/Utilities/DisplayQueue.h"
#include "../Screens/Utilities/ScreenUpdateKeys.h"


class OvenController
{
private:
	TaskHandle_t _taskHandle = nullptr;
	//bool _ovenEnabled = true;
	//portMUX_TYPE _ovenEnabledMutex;

	OvenStatus _ovenStatus = OS_IDLE;
	REFLOW_PHASE _reflowPhase = RP_NOT_ACTIVE;

	Bounce2::Button _stopButton;

	bool _heatersOn = false;
	bool _convectionFanOn = false;
	bool _convectionFanOnManual = false;

	double _temperaturePrimary = 30;
	double _temperatureSecondary = 5;
	bool _streamTemperatures = false;
	bool _testDirection = true;

	//PID
	PID_v2* _pidController = nullptr;
	double _kp;
	double _ki;
	double _kd;

	Profile _profile;

	void CheckConvectionFanStatus();
	void EnableConvectionFan();
	void DisableConvectionFan();
	void EnableOvenHeaters();
	void DisableOvenHeaters();

	void FetchPrimaryTemperature();
	void FetchSecondaryTemperature();
	void FetchTemperatures();
	unsigned long _nextTemperatureDisplayUpdate = 0;


	void HandleOvenHeatersWithPID();
	void HandleReflowSession();
	//void EnableDisableOven(bool state);
public:
	bool Init();
	void Run();

	void StopOven();

	void StartReflowSession(String profileFileName);
	void StartManualHeat(uint16_t targetTemperature);
	void StartTemperatureStream() { _streamTemperatures = true; DisplayQueue.QueueKey(suk_TemperatureStreamStarted); }
	void StopTemperatureStream() { _streamTemperatures = false; }
	//void DisableOven() { EnableDisableOven(false); }
	//void EnableOven() { EnableDisableOven(true); }
	//bool IsOvenEnabled();
};

extern OvenController OvenManager;
void static IRAM_ATTR OvenControllerTask(void*)
{
	OvenManager.Run();
}