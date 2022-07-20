#pragma once
#include <Arduino.h>
#include <TFT_eSPI.h>
#include "Screens_Config.h"
#include "../DisplayManager/Utilities/DisplayQueue.h"
#include "Utilities/DMCoordinates.h"
#include "Widgets/Box.h"
#include "Widgets/TextBox.h"
#include "Widgets/ProgressBar.h"

#define BOX_WIDTH 220
#define BOX_HEIGHT 210
#define NETWORK_LABEL "Network"
#define TB_WIDTH 160
#define TB_X 50
#define LABEL_X (TB_X - 4)
#define SSID_BOX_Y  (LARGE_FONT_TEXT_BOX_H + MEDIUM_FONT_TEXT_BOX_H)
#define LABEL_Y (((SSID_BOX_Y - LARGE_FONT_TEXT_BOX_H) / 2) + LARGE_FONT_TEXT_BOX_H + 2)
#define IP_BOX_Y (SSID_BOX_Y + MEDIUM_FONT_TEXT_BOX_H)
#define IP_BOX_BOTTOM (IP_BOX_Y + SMALL_FONT_TEXT_BOX_H)

#define PROGRESSBAR_Y (IP_BOX_BOTTOM + 40)
#define PROGRESSBAR_WIDTH 206
#define PROGRESSBAR_X ((BOX_WIDTH - PROGRESSBAR_WIDTH)/2)

#define PROGRESSBAR_TEXT_Y (PROGRESSBAR_Y - 15)
#define COMPLETE_TEXT_HEIGHT 17


class OTAScreen
{
private:
	TFT_eSPI* _tft;
	TextBox* _header = nullptr;
	TextBox* _networkName = nullptr;
	TextBox* _ipAddress = nullptr;
	ProgressBar* _progressBar = nullptr;
	int _mainBoxX;
	int _mainBoxY;

	void Draw();
	void OTAComplete();
public:
	OTAScreen(TFT_eSPI* tft);
	~OTAScreen();

	void UpdateScreen(int inKey, char* value);
};


static OTAScreen* otaScreen = nullptr;
static void Create_OTAScreen(TFT_eSPI* TFT) 
{
	otaScreen = new OTAScreen(TFT);
}

static void Destroy_OTAScreen()
{
	if (otaScreen != nullptr)
	{
		delete(otaScreen);
		otaScreen = nullptr;
	}
}

static void Update_OTAScreen(int inKey, char* value)
{
	otaScreen->UpdateScreen(inKey, value);
}