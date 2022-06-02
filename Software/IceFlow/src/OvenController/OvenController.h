#pragma once
#include <Arduino.h>
#include "../IceFlow_Config.h"

class OvenController
{
private:
	bool _ovenOn = false;
	bool _convectionFanOn = false;
	bool _convectionFanOnManual = false;

	uint16_t _temperaturePrimary = 0;
	uint16_t _temperatureSecondary = 0;

	void CheckConvectionFanStatus();

public:
	bool Init();
	void Run();

	void EnableConvectionFan();
	void DisableConvectionFan();
	void EnableOvenHeaters();
	void DisableOvenHeaters();
};

