#pragma once
#include <Arduino.h>
#include "../IceFlow_Config.h"
#include "../Utilities/IceFS.h"

#define PID_DATA_FILENAME "pid.json"

class PidData
{
public:
	float midKp = 0.0;
	float midKi = 0.0;
	float midKd = 0.0;

	float lowKp = 0.0;
	float lowKi = 0.0;
	float lowKd = 0.0;

	float highKp = 0.0;
	float highKi = 0.0;
	float highKd = 0.0;

	PidData();
	~PidData();
	bool GetData();
};

