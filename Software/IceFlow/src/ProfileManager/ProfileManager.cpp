#include "ProfileManager.h"
#include "../Utilities/PreferencesManager.h"
#include "../Utilities/IceFS.h"
#include <ArduinoJson.h>

vector<String> ProfileManager::GetListOfProfileFileNames()
{
	return GetDirectoryListing(PROFILE_SPIFFS_FILE_DIRECTORY);
}

bool ProfileManager::SaveProfileToDisk(Profile profile)
{
	return true;
}

bool ProfileManager::GetProfile(String profileFileName, Profile* profile)
{
	if (profileFileName.isEmpty()) {
		return false;
	}

	String jsonString = IceFS_ReadFile(profileFileName);
	//Serial.println("String: ");
	//Serial.println(jsonString);

	DynamicJsonDocument doc(512);
	DeserializationError error = deserializeJson(doc, jsonString);
	if (error)
	{
		Serial.print("Failed to deserialize doc: ");
		Serial.println(error.code());
		return false;
	}
	
	profile->name							= doc["name"].as<String>();
	profile->pre_heat_target_temperature		= doc["preheat"]["target_temperature"].as<uint16_t>();
	profile->pre_heat_ramp_time				= doc["preheat"]["ramp_time"].as<uint16_t>();
	profile->pre_heat_soak_time				= doc["soak"]["soak_time"].as<uint16_t>();
	profile->pre_heat_soak_end_temperature	= doc["soak"]["soak_end_temperature"].as<uint16_t>();
	profile->reflow_target_temperature		= doc["ramp_to_reflow"]["target_reflow_temperature"].as<uint16_t>();
	profile->reflow_ramp_time				= doc["ramp_to_reflow"]["ramp_time"].as<uint16_t>();
	profile->reflow_max_temperature			= doc["reflow"]["max_reflow_temperature"].as<uint16_t>();
	profile->reflow_soak_time				= doc["reflow"]["reflow_soak_time"].as<uint16_t>();
	profile->cooling_ramp_down_speed			= doc["cooling"]["ramp_down_speed_degrees_per_second"].as<uint16_t>();

	//Serial.print("Profile: ");
	//Serial.println(profile.toJsonString());
	return true;
}

String ProfileManager::GetNameOfLastLoadedProfile()
{
	return GetSavedProfile();
}

void ProfileManager::SaveProfileNameToPreferences(String profileFileName)
{
	SaveProfileName(profileFileName);
}

ProfileManager g_profileManager;