#include "ProfileManager.h"
#include "../Utilities/PreferencesManager.h"
#include <ArduinoJson.h>

int IceFlowProfileManager::GetListOfProfileFileNames(String*& strArray)
{
	return IceFS_GetDirectoryListing(PROFILE_SPIFFS_FILE_DIRECTORY, strArray);
}

bool IceFlowProfileManager::SaveProfileToDisk(Profile profile)
{
	return IceFS_WriteFile(String(PROFILE_SPIFFS_FILE_DIRECTORY) + profile.filename, profile.toJsonString());
}

bool IceFlowProfileManager::GetProfile(String profileFileName, Profile* profile)
{
	if (profileFileName.isEmpty()) {
		return false;
	}

	String jsonString = IceFS_ReadFile(String(PROFILE_SPIFFS_FILE_DIRECTORY) + profileFileName);
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

	profile->name = doc["name"].as<String>();
	profile->pre_heat_target_temperature = doc["preheat"]["target_temperature"].as<uint16_t>();
	profile->pre_heat_ramp_time = doc["preheat"]["ramp_time"].as<uint16_t>();
	profile->pre_heat_soak_time = doc["soak"]["soak_time"].as<uint16_t>();
	profile->pre_heat_soak_end_temperature = doc["soak"]["soak_end_temperature"].as<uint16_t>();
	profile->reflow_target_temperature = doc["ramp_to_reflow"]["target_reflow_temperature"].as<uint16_t>();
	profile->reflow_ramp_time = doc["ramp_to_reflow"]["ramp_time"].as<uint16_t>();
	profile->reflow_max_temperature = doc["reflow"]["max_reflow_temperature"].as<uint16_t>();
	profile->reflow_soak_time = doc["reflow"]["reflow_soak_time"].as<uint16_t>();
	profile->cooling_ramp_down_speed = doc["cooling"]["ramp_down_speed_degrees_per_second"].as<uint16_t>();
	profile->filename = profileFileName;
	//Serial.print("Profile: ");
	//Serial.println(profile.toJsonString());
	return true;
}

bool IceFlowProfileManager::GetSavedProfile(Profile* profile)
{
	return GetProfile(GetNameOfLastLoadedProfile(), profile);
}

String IceFlowProfileManager::GetNameOfLastLoadedProfile()
{
	return GetSavedProfileFileName();
}

void IceFlowProfileManager::SaveProfileFileNameToPreferences(String profileFileName)
{
	SaveProfileFilename(profileFileName);
}

String IceFlowProfileManager::GetNameOfProfileByFileName(String profileFileName)
{
	Profile profile;
	if (GetProfile(profileFileName, &profile)) {
		return profile.name;
	}
	return "";
}

bool IceFlowProfileManager::DeleteProfile(String profileFileName)
{
	String fullPathToFile = String(PROFILE_SPIFFS_FILE_DIRECTORY) + profileFileName;
	return IceFS_DeleteFile(fullPathToFile);
}

bool IceFlowProfileManager::SaveProfileAsNewToDisk(Profile profile)
{
	String fileName = profile.name.substring(0, MAXIMUM_PROFILE_FILE_NAME_LENGTH - 1);
	fileName.replace(" ", "_");
	int fileCounter = 0;
	String tmpFileName = fileName + String(FILENAME_EXTENSION);

	bool found = IceFs_DoesFileExist(String(PROFILE_SPIFFS_FILE_DIRECTORY) + tmpFileName);
	while (found) {
		fileCounter++;
		fileName.remove(fileName.length() - 1);
		tmpFileName = fileName + String(fileCounter) + String(FILENAME_EXTENSION);
		found = IceFs_DoesFileExist(String(PROFILE_SPIFFS_FILE_DIRECTORY) + tmpFileName);
	}
	
	profile.filename = tmpFileName;
	bool ret = IceFS_WriteFile(String(PROFILE_SPIFFS_FILE_DIRECTORY) + profile.filename, profile.toJsonString());
	if (ret) {
		SaveProfileFileNameToPreferences(profile.filename);
	}
	return ret;
}

IceFlowProfileManager ProfileManager;