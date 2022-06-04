#pragma once
#include <Arduino.h>
#include "ScreenBase.h"
#include "Panels/TemperaturePanel.h"

class PreHeatScreen :
    public ScreenBase
{
private:
	TemperaturePanel _temperaturePanel;

	void DrawScreen();
public:
	PreHeatScreen(TFT_eSPI* newTFT);
	~PreHeatScreen();


	void UpdateScreen(DISPLAY_UPDATE_KEYS inKey, String inPayload);
	void UpdateScreenOnInterval();
	void HandleTouch(int x, int y);
};


static PreHeatScreen* preHeatScreen = NULL;

static void Create_PreHeatScreen(TFT_eSPI* TFT) {
	preHeatScreen = new PreHeatScreen(TFT);
}

static void Destroy_PreHeatScreen()
{
	if (preHeatScreen != NULL)
	{
		delete(preHeatScreen);
		preHeatScreen = NULL;
	}
}

static void Update_PreHeatScreen(DISPLAY_UPDATE_KEYS inKey, String inPayload)
{
	preHeatScreen->UpdateScreen(inKey, inPayload);
}

static void Update_PreHeatScreen_OnInterval()
{
	preHeatScreen->UpdateScreenOnInterval();
}

static void Handle_PreHeatScreen_Touch(int x, int y)
{
	preHeatScreen->HandleTouch(x, y);
}