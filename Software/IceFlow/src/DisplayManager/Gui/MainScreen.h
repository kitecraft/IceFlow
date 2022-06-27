#pragma once
#include "ScreenBase.h"
#include "../../ProfileManager/ProfileManager.h"
#include "Panels/MainScreenSideBar.h"
#include "Panels/TemperaturePanel.h"
#include "Panels/MainScreenHeader.h"
#include "Panels/CenterPanel_Normal.h"


class MainScreen :
    public ScreenBase
{
private:
	MainScreenHeader _msmHeader;
	MainScreenSideBar _sideBar;
	TemperaturePanel _temperaturePanel;
	CenterPanel_Normal _cpNormal;
	String _currentProfileFileName = "";
	Profile _currentProfile;

	void LoadProfile();

	void DrawScreen();

public:
	MainScreen(TFT_eSPI* newTFT);
	~MainScreen();


	void UpdateScreen(DISPLAY_UPDATE_KEYS inKey, String inPayload);
	void UpdateScreenOnInterval();
	void HandleTouch(int x, int y);
};


static MainScreen* mainScreen = NULL;

static void Create_MainScreen(TFT_eSPI* TFT) {
	mainScreen = new MainScreen(TFT);
}

static void Destroy_MainScreen()
{
	if (mainScreen != NULL)
	{
		delete(mainScreen);
		mainScreen = NULL;
	}
}

static void Update_MainScreen(DISPLAY_UPDATE_KEYS inKey, String inPayload)
{
	mainScreen->UpdateScreen(inKey, inPayload);
}

static void Update_MainScreen_OnInterval()
{
	mainScreen->UpdateScreenOnInterval();
}

static void Handle_MainScreen_Touch(int x, int y)
{
	mainScreen->HandleTouch(x, y);
}