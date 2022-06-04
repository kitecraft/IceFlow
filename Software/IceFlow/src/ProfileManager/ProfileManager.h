#pragma once
#include <Arduino.h>
#include "Profile.h"

using namespace std;
#include <vector>

class ProfileManager
{
private:

public:
	vector<String> GetListOfProfileFileNames();
	bool SaveProfileToDisk(Profile profile); //saved as Profile <name>.json with spaces to _
	bool GetProfile(String profileName, Profile *profile); // 
	String GetNameOfLastLoadedProfile();
	void SaveProfileNameToPreferences(String profileFileName);
};

