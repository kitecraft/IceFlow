#pragma once
#include <Arduino.h>
#include "../Utilities/IceFS.h"
#include "Profile.h"


#define PROFILE_SPIFFS_FILE_DIRECTORY "/p/"
#define MAXIMUM_PROFILE_FILE_NAME_LENGTH 8
#define PROFILE_FILENAME_EXTENSION ".json"

class IceFlowProfileManager
{
private:
	String GetNameOfLastLoadedProfile();

public:
	void SaveProfileFileNameToPreferences(String profileFileName);
	bool SaveProfileToDisk(Profile profile);
	bool SaveProfileAsNewToDisk(Profile profile);

	int GetListOfProfileFileNames(String*& strArray);
	bool GetProfile(String profileFileName, Profile* profile);
	bool GetSavedProfile(Profile* profile);

	String GetNameOfProfileByFileName(String profileFileName);
	double GetFreeSpaceKB() { return (IceFS_GetFreespaceBytes() / 1024.0); }

	bool DeleteProfile(String profileFileName);
};

extern IceFlowProfileManager ProfileManager;