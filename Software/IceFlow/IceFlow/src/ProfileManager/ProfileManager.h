#pragma once
#include <Arduino.h>
#include "../Utilities/IceFS.h"
#include "Profile.h"
#include "../IceFlow_Config.h"


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