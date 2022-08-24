#pragma once
#include <Arduino.h>
#include "../IceFlow_Config.h"
#include "Widgets/Button.h"
#include "Utilities/DMTheme.h"
#include "Utilities/DMCoordinates.h"

#define IS_HEADER_X 0
#define IS_HEADER_Y 0
#define IS_HEADER_W 320
#define IS_HEADER_H (MEDIUM_FONT_TEXT_BOX_H + 10)

#define IS_EXIT_BUTTON_W 65
#define IS_EXIT_BUTTON_H 23
#define IS_EXIT_BUTTON_X (320 - IS_EXIT_BUTTON_W)
#define IS_EXIT_BUTTON_Y (240 - IS_EXIT_BUTTON_H)

#define IS_DEVICE_NAME_X ((IS_HEADER_H/2) - 3)
#define IS_DEVICE_NAME_Y 7
#define IS_AUTHOR_NAME_X 160

#define IS_NETWORK_BOX_X 0
#define IS_NETWORK_BOX_Y IS_HEADER_H
#define IS_NETWORK_BOX_W 320
#define IS_NETWORK_BOX_H 60

class InfoScreen
{
private:
	TFT_eSPI* _tft;
	Button* _exitButton = nullptr;


	void Draw();
	void DrawHeader();
	
	void ProcessTouch(int x, int y);
public:
	InfoScreen(TFT_eSPI* tft);
	~InfoScreen();

	void UpdateScreen(int inKey, char* value);
	//void UpdateScreenOnInterval();

	void HandleTouch(int x, int y);
};



static InfoScreen* infoScreen = nullptr;
static void Create_InfoScreen(TFT_eSPI* TFT)
{
	infoScreen = new InfoScreen(TFT);
}

static void Destroy_InfoScreen()
{
	if (infoScreen != nullptr)
	{
		delete(infoScreen);
		infoScreen = nullptr;
	}
}

static void Update_InfoScreen(int inKey, char* value)
{
	infoScreen->UpdateScreen(inKey, value);
}

/*
static void Update_InfoScreen_OnInterval()
{
	infoScreen->UpdateScreenOnInterval();
}
*/

static void HandleTouch_InfoScreen(int x, int y)
{
	infoScreen->HandleTouch(x, y);
}
