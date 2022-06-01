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
	Profile GetProfile(String profileName); // 
	String GetNameOfLastLoadedProfile();
	bool SaveNameToLastLoadedProfile(String profileFileName);
};

