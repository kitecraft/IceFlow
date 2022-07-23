#pragma once
#include <Arduino.h>
#include <TFT_eSPI.h>
#include "Utilities/Screens_Config.h"
#include "Panels/SideBar.h"

#define WIFI_Y 6
#define WIFI_SPOT_R 4
#define TIME_Y 0
#define TIME_W 38
#define TIME_H 10


#define SIDEBAR_Y (TIME_H + 4)

class MainScreen
{
private:
	TFT_eSPI* _tft;
	TFT_eSprite* _timeSprite = nullptr;
	uint16_t* _timeSprPtr;
	int _wifiX;
	int _timeX;

	SideBar* _sideBar = nullptr;
	int _sidebarX;

	void DrawScreen();
	void DisplayTime();
	void IRAM_ATTR OnTimer();
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


