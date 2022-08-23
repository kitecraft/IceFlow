#include "PidData.h"
#include <ArduinoJson.h>

PidData::PidData()
{

}

PidData::~PidData()
{

}

bool PidData::GetData()
{
	String jsonString = IceFS_ReadFile(String(PID_DATA_SPIFFS_FILE_DIRECTORY) + PID_DATA_FILENAME);
	if (jsonString.isEmpty()) {
		return false;
	}

	DynamicJsonDocument doc(512);
	DeserializationError error = deserializeJson(doc, jsonString);
	if (error)
	{
		Serial.print("Failed to deserialize doc: ");
		Serial.println(error.code());
		return false;
	}

	midKp = doc["mid"]["kp"].as<float>();
	midKi = doc["mid"]["ki"].as<float>();
	midKd = doc["mid"]["kd"].as<float>();

	lowKp = doc["low"]["kp"].as<float>();
	lowKi = doc["low"]["ki"].as<float>();
	lowKd = doc["low"]["kd"].as<float>();

	highKp = doc["high"]["kp"].as<float>();
	highKi = doc["high"]["ki"].as<float>();
	highKd = doc["high"]["kd"].as<float>();

	return true;
}

bool PidData::SaveData()
{
	return IceFS_WriteFile(String(PID_DATA_SPIFFS_FILE_DIRECTORY) + PID_DATA_FILENAME, ToJsonString());
}

String PidData::ToJsonString()
{
	String ret = "{\"mid\":{\"kp\":" + String(midKp) + ",";
	ret += "\"ki\":" + String(midKi) + ",";
	ret += "\"kd\":" + String(midKd) + "},";

	ret += "\"low\":{\"kp\":" + String(lowKp) + ",";
	ret += "\"ki\":" + String(lowKi) + ",";
	ret += "\"kd\":" + String(lowKd) + "},";

	ret += "\"high\":{\"kp\":" + String(highKp) + ",";
	ret += "\"ki\":" + String(highKi) + ",";
	ret += "\"kd\":" + String(highKd) + "}}";

	return ret;
}