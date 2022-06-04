#pragma once
#include <Arduino.h>
#include <PID_v2.h>
#include "../IceFlow_Config.h"
#include "OvenEnums.h"
#include "../DisplayManager/Utilities/DisplayQueue.h"
#include "../DisplayManager/Utilities/Display_Enums.h"
#include "../ProfileManager/ProfileManager.h"
#include "../ProfileManager/Profile.h"

extern DisplayQueue g_displayQueue;
extern ProfileManager g_profileManager;

class OvenController
{
private:
	bool _ovenEnabled = true;
	portMUX_TYPE _ovenEnabledMutex;

	OvenStatus _ovenStatus = OS_IDLE;
	REFLOW_PHASE _reflowPhase = RP_NOT_ACTIVE;

	bool _heatersOn = false;
	bool _convectionFanOn = false;
	bool _convectionFanOnManual = false;

	double _temperaturePrimary = 50;
	bool _testDirection = true;
	double _temperatureSecondary = 0;

	//PID
	PID_v2 *_pidController = nullptr;
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
	void EnableDisableOven(bool state);
public:
	bool Init();
	void Run();

	void EmergencyStop();

	void StartReflowSession(String profileFileName);
	void StartManualPreHeat(uint16_t targetTemperature);
	void DisableOven() { EnableDisableOven(false); }
	void EnableOven() { EnableDisableOven(true); }
	bool IsOvenEnabled();
};

