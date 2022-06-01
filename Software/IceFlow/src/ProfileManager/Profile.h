#pragma once
#include <Arduino.h>

class Profile
{
public:
	uint16_t pre_heat_target_temperature = 0;
	uint16_t pre_heat_ramp_time = 0;
	uint16_t pre_heat_soak_time = 60;
	uint16_t pre_heat_soak_end_temperature = 0;
	uint16_t reflow_target_temperature = 0;
	uint16_t reflow_ramp_time = 0;
	uint16_t reflow_max_temperature = 0;
	uint16_t reflow_soak_time = 0;
	uint16_t cooling_ramp_down_speed = 0;
};

