#pragma once
#include <Arduino.h>

class Profile
{
public:
	String name = "Not Loaded";
	uint16_t pre_heat_target_temperature = 0;
	uint16_t pre_heat_ramp_time = 0;
	uint16_t pre_heat_soak_time = 60;
	uint16_t pre_heat_soak_end_temperature = 0;
	uint16_t reflow_target_temperature = 0;
	uint16_t reflow_ramp_time = 0;
	uint16_t reflow_max_temperature = 0;
	uint16_t reflow_soak_time = 0;
	uint16_t cooling_ramp_down_speed = 0;

	String toJsonString()
	{
		String ret = "{\"name\":\"" + name + "\",";
		ret += "\"pretheat\" : {";
		ret += "\"target_temperature\" : " + String(pre_heat_target_temperature) + ",";
		ret += "\"ramp_time\" : " + String(pre_heat_ramp_time) + "},";
		ret += "\"soak\" : {";
		ret += "\"soak_time\" : " + String(pre_heat_soak_time) + ",";
		ret += "\"soak_end_temperature\" : " + String(pre_heat_soak_end_temperature) + "},";
		ret += "\"ramp_to_reflow\" : {";
		ret += "\"target_reflow_temperature\" : " + String(reflow_target_temperature) + ",";
		ret += "\"ramp_time\" : " + String(reflow_ramp_time) + "},";
		ret += "\"reflow\" : {";
		ret += "\"max_reflow_temperature\" : " + String(reflow_max_temperature) + ",";
		ret += "\"reflow_soak_time\" : " + String(reflow_soak_time) + "},";
		ret += "\"cooling\" : {";
		ret += "\"ramp_down_speed_degrees_per_second\" : " + String(cooling_ramp_down_speed) + "}}";

		return ret;
	}
};
