#pragma once
#include "ScreenBase.h"
#include "../../ProfileManager/ProfileManager.h"
#include "../../ProfileManager/Profile.h"
#include "Panels/MainScreenSideBar.h"
#include "Panels/TemperaturePanel.h"

extern ProfileManager g_profileManager;

class MainScreen :
    public ScreenBase
{
private:
	MainScreenSideBar _sideBar;
	TemperaturePanel _temperaturePanel;
	String _currentProfileFileName = "";

	void DrawScreen();
	void DrawProfile();

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