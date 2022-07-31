#pragma once
#include <Arduino.h>
#include <PID_v2.h>
#include "../IceFlow_Config.h"
#include "OvenEnums.h"
#include "../ProfileManager/ProfileManager.h"



class OvenController
{
private:
	TaskHandle_t _taskHandle = nullptr;
	//bool _ovenEnabled = true;
	//portMUX_TYPE _ovenEnabledMutex;

	OvenStatus _ovenStatus = OS_IDLE;
	REFLOW_PHASE _reflowPhase = RP_NOT_ACTIVE;

	bool _heatersOn = false;
	bool _convectionFanOn = false;
	bool _convectionFanOnManual = false;

	double _temperaturePrimary = 50;
	double _temperatureSecondary = 0;
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


	void HandleManualPreHeat();
	void HandleReflowSession();
	//void EnableDisableOven(bool state);
public:
	bool Init();
	void Run();

	void EmergencyStop();

	void StartReflowSession(String profileFileName);
	void StartManualPreHeat(uint16_t targetTemperature);
	void StartTemperatureStream() { _streamTemperatures = true; }
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