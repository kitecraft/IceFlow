#pragma once
#include <Arduino.h>
#include <TFT_eSPI.h>

#define WIFI_Y 0

class MainScreen
{
private:
	TFT_eSPI* _tft;
	int _wifiX;
	void DrawScreen();
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


