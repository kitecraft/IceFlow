#pragma once
#include <Arduino.h>

#define NO_PROFILE_FOUND "No Profile"

class Profile
{
public:
	String name = NO_PROFILE_FOUND;
	uint16_t pre_heat_target_temperature = 0;
	uint16_t pre_heat_ramp_time = 0;
	uint16_t pre_heat_soak_time = 60;
	uint16_t pre_heat_soak_end_temperature = 0;
	uint16_t reflow_target_temperature = 0;
	uint16_t reflow_ramp_time = 0;
	uint16_t reflow_max_temperature = 0;
	uint16_t reflow_soak_time = 0;
	uint16_t cooling_ramp_down_speed = 0;
	String filename = "";

	Profile() {}

	Profile(const Profile& orig) {
		name = orig.name;
		pre_heat_target_temperature = orig.pre_heat_target_temperature;
		pre_heat_ramp_time = orig.pre_heat_ramp_time;
		pre_heat_soak_time = orig.pre_heat_soak_time;
		pre_heat_soak_end_temperature = orig.pre_heat_soak_end_temperature;
		reflow_target_temperature = orig.reflow_target_temperature;
		reflow_ramp_time = orig.reflow_ramp_time;
		reflow_max_temperature = orig.reflow_max_temperature;
		reflow_soak_time = orig.reflow_soak_time;
		cooling_ramp_down_speed = orig.cooling_ramp_down_speed;
		filename = orig.filename;
	}

	bool operator==(Profile& other) {
		if (
			name == other.name &&
			pre_heat_target_temperature == other.pre_heat_target_temperature &&
			pre_heat_ramp_time == other.pre_heat_ramp_time &&
			pre_heat_soak_time == other.pre_heat_soak_time &&
			pre_heat_soak_end_temperature == other.pre_heat_soak_end_temperature &&
			reflow_target_temperature == other.reflow_target_temperature &&
			reflow_ramp_time == other.reflow_ramp_time &&
			reflow_max_temperature == other.reflow_max_temperature &&
			reflow_soak_time == other.reflow_soak_time &&
			cooling_ramp_down_speed == other.cooling_ramp_down_speed &&
			filename == other.filename) {
			return true;
		}
		return false;
	}

	bool operator!=(Profile& other) {
		if (
			name == other.name &&
			pre_heat_target_temperature == other.pre_heat_target_temperature &&
			pre_heat_ramp_time == other.pre_heat_ramp_time &&
			pre_heat_soak_time == other.pre_heat_soak_time &&
			pre_heat_soak_end_temperature == other.pre_heat_soak_end_temperature &&
			reflow_target_temperature == other.reflow_target_temperature &&
			reflow_ramp_time == other.reflow_ramp_time &&
			reflow_max_temperature == other.reflow_max_temperature &&
			reflow_soak_time == other.reflow_soak_time &&
			cooling_ramp_down_speed == other.cooling_ramp_down_speed &&
			filename == other.filename) {
			return false;
		}
		return true;
	}

	String toJsonString()
	{
		String ret = "{\"name\":\"" + name + "\",";
		ret += "\"preheat\" : {";
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
