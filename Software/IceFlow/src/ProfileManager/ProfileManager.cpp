#include "ProfileManager.h"
#include "../Utilities/PreferencesManager.h"
#include "../Utilities/IceFS.h"

vector<String> ProfileManager::GetListOfProfileFileNames()
{
	return GetDirectoryListing(PROFILE_SPIFFS_FILE_DIRECTORY);
}

bool ProfileManager::SaveProfileToDisk(Profile profile)
{
	return true;
}

Profile ProfileManager::GetProfile(String profileFileName)
{
	String jsonFile = IceFS_ReadFile(profileFileName);

	return Profile();
}

String ProfileManager::GetNameOfLastLoadedProfile()
{

	return "";
}

bool ProfileManager::SaveNameToLastLoadedProfile(String profileFileName)
{
	return true;
}
