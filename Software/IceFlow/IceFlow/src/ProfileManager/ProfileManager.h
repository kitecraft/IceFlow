#pragma once
#include <Arduino.h>
#include "Profile.h"

using namespace std;
#include <vector>

#define PROFILE_SPIFFS_FILE_DIRECTORY "/p/"

class IceFlowProfileManager
{
private:

public:
	vector<String> GetListOfProfileFileNames();
	bool SaveProfileToDisk(Profile profile); //saved as Profile <name>.json with spaces to _
	bool GetProfile(String profileName, Profile* profile); // 
	String GetNameOfLastLoadedProfile();
	void SaveProfileNameToPreferences(String profileFileName);
};

extern IceFlowProfileManager ProfileManager;