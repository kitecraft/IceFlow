#pragma once
#include <Arduino.h>
#include <TFT_eSPI.h>
#include "../IceFlow_Config.h"
#include "Utilities/Screens_Config.h"
#include "Panels/SideBar.h"
#include "Widgets/TextBox.h"
#include "../ProfileManager/ProfileManager.h"

#define WIFI_Y 6
#define WIFI_SPOT_R 4
#define TIME_Y 0
#define TIME_W 38
#define TIME_H 10

#define SIDEBAR_Y (TIME_H + 4)

#define HEADER_X 0
#define HEADER_Y 0
#define HEADER_W_OFFSET 3
#define HEADER_H (MEDIUM_FONT_TEXT_BOX_H + 10)

#define DEVICE_NAME_TB_X ((HEADER_H/2) - 3)
#define HEADER_TB_Y 5

#define PROFILE_LABEL_X_OFFSET 15
#define PROFILE_LABEL "Profile "

class MainScreen
{
private:
	TFT_eSPI* _tft;
	TFT_eSprite* _timeSprite = nullptr;
	int _wifiX;
	int _timeX;
	int _headerW;

	SideBar* _sideBar = nullptr;
	int _sidebarX;

	Profile _currentProfile;

	void DrawScreen();
	void DrawHeader();
	void DisplayTime();

public:
	MainScreen(TFT_eSPI* tft);
	~MainScreen();

	void UpdateScreen(int inKey, char* value);
	void UpdateScreenOnInterval();

	void HandleTouch(int x, int y);

};


static MainScreen* mainScreen = nullptr;
static void Create_MainScreen(TFT_eSPI* TFT)
{
	mainScreen = new MainScreen(TFT);
}

static void Destroy_MainScreen()
{
	if (mainScreen != nullptr)
	{
		delete(mainScreen);
		mainScreen = nullptr;
	}
}

static void Update_MainScreen(int inKey, char* value)
{
	mainScreen->UpdateScreen(inKey, value);
}

static void Update_MainScreenOnInterval()
{
	mainScreen->UpdateScreenOnInterval();
}

static void HandleTounch_MainScreen(int x, int y)
{
	mainScreen->HandleTouch(x, y);
}


