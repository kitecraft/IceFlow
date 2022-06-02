#pragma once
#include <Arduino.h>
#include <PID_v2.h>
#include "../IceFlow_Config.h"
#include "OvenEnums.h"

extern DisplayQueue g_displayQueue;

class OvenController
{
private:
	OvenStatus _ovenStatus = OS_IDLE;
	REFLOW_PHASE _reflowPhase = RP_NOT_ACTIVE;

	bool _heatersOn = false;
	bool _convectionFanOn = false;
	bool _convectionFanOnManual = false;

	double _temperaturePrimary = 0;
	double _temperatureSecondary = 0;

	//PID
	PID_v2 *_pidController = nullptr;
	double _kp;
	double _ki;
	double _kd;

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
public:
	bool Init();
	void Run();

	void EmergencyStop() { DisableOvenHeaters(); DisableConvectionFan(); }

	void StartReflowSession(String profileFileName);
	void StartManualPreHeat(uint16_t targetTemperature);
};

