#pragma once
#include <Arduino.h>
#include "Profile.h"


#define PROFILE_SPIFFS_FILE_DIRECTORY "/p/"

class IceFlowProfileManager
{
private:

public:
	int GetListOfProfileFileNames(String* strArray);
	bool SaveProfileToDisk(Profile profile); //saved as Profile <name>.json with spaces to _
	bool GetProfile(String profileName, Profile* profile); // 
	bool GetSavedProfile(Profile* profile);
	String GetNameOfLastLoadedProfile();
	void SaveProfileNameToPreferences(String profileFileName);
};

extern IceFlowProfileManager ProfileManager;