#pragma once
#include <Arduino.h>
#include <TFT_eSPI.h>
#include "Screens_Config.h"
#include "../DisplayManager/Utilities/DisplayQueue.h"
#include "Utilities/DMCoordinates.h"
#include "Widgets/Box.h"
#include "Widgets/TextBox.h"
#include "Widgets/Spinner.h"

#define BOX_WIDTH 220
#define BOX_HEIGHT 210
#define NETWORK_LABEL "Network"
#define TB_WIDTH 160
#define TB_X 50
#define LABEL_X (TB_X - 4)
#define SSID_BOX_Y  (LARGE_FONT_TEXT_BOX_H + MEDIUM_FONT_TEXT_BOX_H)
#define IP_BOX_Y (SSID_BOX_Y + MEDIUM_FONT_TEXT_BOX_H)
#define IP_BOX_BOTTOM (IP_BOX_Y + SMALL_FONT_TEXT_BOX_H)
#define SPINNER_Y (((BOX_HEIGHT - IP_BOX_BOTTOM) / 2) + IP_BOX_BOTTOM)

#define LABEL_Y (((SSID_BOX_Y - LARGE_FONT_TEXT_BOX_H) / 2) + LARGE_FONT_TEXT_BOX_H + 2)

class SplashScreen
{
private:
	TFT_eSPI* _tft;
	TextBox *_networkName = nullptr;
	TextBox *_ipAddress = nullptr;
	TextBox *_header = nullptr;
	Spinner* _spinner = nullptr;
	int _mainBoxX;
	int _mainBoxY;

	void Draw();
public:
	SplashScreen(TFT_eSPI* tft);
	~SplashScreen();

	void UpdateScreen(int inKey, char* value);
	void UpdateScreenOnInterval();
};


static SplashScreen* splashScreen = nullptr;
static void Create_SplashScreen(TFT_eSPI* TFT) {
	splashScreen = new SplashScreen(TFT);
}

static void Destroy_SplashScreen()
{
	if (splashScreen != nullptr)
	{
		delete(splashScreen);
		splashScreen = nullptr;
	}
}

static void Update_SplashScreen(int inKey, char* value)
{
	splashScreen->UpdateScreen(inKey, value);
}

static void Update_SplashScreen_OnInterval()
{
	splashScreen->UpdateScreenOnInterval();
}
