#pragma once
#include <Arduino.h>
#include <TFT_eSPI.h>
#include "../IceFlow_Config.h"
#include "Utilities/Screens_Config.h"
#include "Panels/SideBar.h"
#include "Widgets/TextBox.h"
#include "../ProfileManager/ProfileManager.h"

#define WIFI_X (320 - 4)
#define WIFI_Y 6
#define WIFI_SPOT_R 4

#define TIME_W 38
#define TIME_X (WIFI_X - TIME_W - 7)
#define TIME_Y 0
#define TIME_H 10

#define SIDEBAR_X (320 - SIDEBAR_W)
#define SIDEBAR_Y (TIME_H + 4)

#define MS_HEADER_X 0
#define MS_HEADER_Y 0
#define MS_HEADER_W_OFFSET 3
#define MS_HEADER_W (SIDEBAR_X - MS_HEADER_W_OFFSET)
#define MS_HEADER_H (MEDIUM_FONT_TEXT_BOX_H + 10)

#define MS_FOOTER_W_OFFSET 3
#define MS_FOOTER_X 0
#define MS_FOOTER_H (MEDIUM_FONT_TEXT_BOX_H + 10)
#define MS_FOOTER_Y (240 - MS_FOOTER_H)
#define MS_FOOTER_W 320 //(SIDEBAR_X - MS_FOOTER_W_OFFSET)

#define MS_PRIMARY_TEMPERATURE_COLOR TFT_RED
#define MS_SEC_TEMPERATURE_COLOR TFT_ORANGE

#define MS_TEMPERATURE_TB_W (MEDIUM_FONT_TEXT_BOX_H + 55)
#define MS_PRIMARY_TEMPERATURE_LABEL_X (MS_HEADER_H / 2)
#define MS_TEMPERATURE_LABEL_Y ((MS_FOOTER_H / 2) - 1)
#define MS_PRIMARY_TEMPERATURE_TB_X ( MS_PRIMARY_TEMPERATURE_LABEL_X + 33)

#define MS_SEC_TEMPERATURE_LABEL_X (MS_PRIMARY_TEMPERATURE_TB_X + MS_TEMPERATURE_TB_W + 5)
#define MS_SEC_TEMPERATURE_TB_X (MS_SEC_TEMPERATURE_LABEL_X + 25)

#define MS_DEVICE_NAME_X ((MS_HEADER_H/2) - 3)
#define MS_DEVICE_NAME_Y 7
#define MS_HEADER_TB_Y 5

#define MS_PROFILE_LABEL_X_OFFSET 15
#define MS_PROFILE_LABEL "Profile "

class MainScreen
{
private:
	TFT_eSPI* _tft;
	TFT_eSprite* _timeSprite = nullptr;
	SideBar* _sideBar = nullptr;
	Profile _currentProfile;

	TextBox* _primaryTemperature;
	TextBox* _secondaryTemperature;

	void DrawScreen();
	void DrawHeader();
	void DrawFooter();
	void DisplayTime();

	void UpdatePrimaryTemp(char* val);
	void UpdateSecondaryTemp(char* val);

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


