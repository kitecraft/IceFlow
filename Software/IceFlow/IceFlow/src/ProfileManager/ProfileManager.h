#pragma once
#include <Arduino.h>
#include "../Utilities/IceFS.h"
#include "Profile.h"


#define PROFILE_SPIFFS_FILE_DIRECTORY "/p/"

class IceFlowProfileManager
{
private:

public:
	int GetListOfProfileFileNames(String*& strArray);
	bool SaveProfileToDisk(Profile profile); //saved as Profile <name>.json with spaces to _
	bool GetProfile(String profileName, Profile* profile); // 
	bool GetSavedProfile(Profile* profile);
	String GetNameOfLastLoadedProfile();
	void SaveProfileNameToPreferences(String profileFileName);
	String GetNameOfProfileByFileName(String profileFileName);
	double GetFreeSpaceKB() { return (GetFreespaceBytes() / 1024.0); }
};

extern IceFlowProfileManager ProfileManager;