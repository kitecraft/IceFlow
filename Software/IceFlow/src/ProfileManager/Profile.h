#pragma once
#include <Arduino.h>

class Profile
{
public:
	String name = "";
	uint16_t pre_heat_target_temperature = 0;
	uint16_t pre_heat_ramp_time = 0;
	uint16_t pre_heat_soak_time = 60;
	uint16_t pre_heat_soak_end_temperature = 0;
	uint16_t reflow_target_temperature = 0;
	uint16_t reflow_ramp_time = 0;
	uint16_t reflow_max_temperature = 0;
	uint16_t reflow_soak_time = 0;
	uint16_t cooling_ramp_down_speed = 0;

	Profile() {};
	Profile(const Profile &origProfile) {
		name = origProfile.name;
		pre_heat_target_temperature = origProfile.pre_heat_target_temperature;
		pre_heat_ramp_time = origProfile.pre_heat_ramp_time;
		pre_heat_soak_time = origProfile.pre_heat_soak_time;
		pre_heat_soak_end_temperature = origProfile.pre_heat_soak_end_temperature;
		reflow_target_temperature = origProfile.reflow_target_temperature;
		reflow_ramp_time = origProfile.reflow_ramp_time;
		reflow_max_temperature = origProfile.reflow_max_temperature;
		reflow_soak_time = origProfile.reflow_soak_time;
		cooling_ramp_down_speed = origProfile.cooling_ramp_down_speed;
	}

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
