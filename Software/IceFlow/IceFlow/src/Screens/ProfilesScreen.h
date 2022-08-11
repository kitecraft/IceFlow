#include <Arduino.h>
#include <TFT_eSPI.h>
#include "../IceFlow_Config.h"
#include "Utilities/Screens_Config.h"
#include "../ProfileManager/ProfileManager.h"
#include "Widgets/Button.h"
#include "Panels/ProfileListPanel.h"
#include "Panels/PS_ProfileGraphPanel.h"

#define HEADER_X 0
#define HEADER_Y 0
#define HEADER_W 320
#define HEADER_W_OFFSET 3

#define HEADER_H (MEDIUM_FONT_TEXT_BOX_H + 10)

#define DEVICE_NAME_X ((HEADER_H/2) - 3)
#define DEVICE_NAME_Y 7
#define HEADER_TB_Y 5

#define PROFILE_LABEL_X_OFFSET 15
#define PROFILE_LABEL "Profile "

#define BUTTON_W (PROFILE_FILE_LISTBOX_W/2)
#define BUTTON_H 21
#define BUTTON_Y (240 - BUTTON_H)
#define EXIT_EDIT_BUTTON_X (PROFILE_FILE_LISTBOX_X + BUTTON_W)
#define SAVE_SET_BUTTON_X PROFILE_FILE_LISTBOX_X

#define SHOW_GRID_BTN_W 30
#define SHOW_GRID_BTN_X (HEADER_W - SHOW_GRID_BTN_W - 11)
#define SHOW_GRID_BTN_Y ((HEADER_H/2) - (BUTTON_H/2))

class ProfilesScreen
{
private:
	TFT_eSPI* _tft = nullptr;
	Profile _currentlyLoadedProfile;
	Profile _currentlySavedProfile;
	ProfileListPanel* _profileListPanel = nullptr;
	Button* _exitButton = nullptr;
	Button* _editButton = nullptr;
	Button* _setButton = nullptr;
	Button* _cancelButton = nullptr;
	bool _setRequired = false;
	Button* _showGridButton = nullptr;

	PS_ProfileGraphPanel* _graphPanel = nullptr;

	void Draw();
	void DrawHeader();
	void ProcessTouch(int x, int y);
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


