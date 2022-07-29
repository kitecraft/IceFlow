#include <Arduino.h>
#include <TFT_eSPI.h>
#include "../IceFlow_Config.h"
#include "Utilities/Screens_Config.h"
#include "../ProfileManager/ProfileManager.h"
#include "Widgets/Button.h"
#include "Panels/ProfileListPanel.h"

#define HEADER_X 0
#define HEADER_Y 0
#define HEADER_W_OFFSET 3
#define HEADER_H (MEDIUM_FONT_TEXT_BOX_H + 10)

#define DEVICE_NAME_TB_X ((HEADER_H/2) - 3)
#define HEADER_TB_Y 5

#define PROFILE_LABEL_X_OFFSET 15
#define PROFILE_LABEL "Profile "

#define BUTTON_W (PROFILE_FILE_LISTBOX_W/2)
#define BUTTON_H 21
#define BUTTON_Y (240 - BUTTON_H)
#define EXIT_BUTTON_X (PROFILE_FILE_LISTBOX_X + BUTTON_W)
#define SAVE_BUTTON_X PROFILE_FILE_LISTBOX_X


class ProfilesScreen
{
private:
	TFT_eSPI* _tft = nullptr;
	Profile _currentlyLoadedProfile;
	Profile _currentlySavedProfile;
	int _headerW;
	ProfileListPanel* _profileListPanel = nullptr;
	Button* _exitButton = nullptr;
	Button* _saveButton = nullptr;
	Button* _cancelButton = nullptr;
	bool _saveRequired = false;

	void Draw();
	void DrawHeader();
public:
	ProfilesScreen(TFT_eSPI* tft);
	~ProfilesScreen();

	void UpdateScreen(int inKey, char* value);
	void UpdateScreenOnInterval();

	void HandleTouch(int x, int y);
};


static ProfilesScreen* profilesScreen = nullptr;
static void Create_ProfilesScreen(TFT_eSPI* TFT)
{
	profilesScreen = new ProfilesScreen(TFT);
}

static void Destroy_ProfilesScreen()
{
	if (profilesScreen != nullptr)
	{
		delete(profilesScreen);
		profilesScreen = nullptr;
	}
}

static void Update_ProfilesScreen(int inKey, char* value)
{
	profilesScreen->UpdateScreen(inKey, value);
}

static void Update_ProfilesScreen_OnInterval()
{
	profilesScreen->UpdateScreenOnInterval();
}

static void HandleTouch_ProfilesScreen(int x, int y)
{
	profilesScreen->HandleTouch(x, y);
}


