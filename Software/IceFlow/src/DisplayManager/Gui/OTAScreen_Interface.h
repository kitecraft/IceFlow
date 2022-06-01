#pragma once
#include "OTAScreen.h"

OTAScreen* otaScreen = NULL;
void Create_OTAScreen(TFT_eSPI* TFT) {
	otaScreen = new OTAScreen(TFT);
}

void Destroy_OTAScreen()
{
	if (otaScreen != NULL)
	{
		delete(otaScreen);
		otaScreen = NULL;
	}
}

void Update_OTAScreen(DISPLAY_UPDATE_KEYS inKey, String inPayload)
{
	otaScreen->UpdateScreen(inKey, inPayload);
}

void Update_OTAScreen_OnInterval()
{
	otaScreen->UpdateScreenOnInterval();
}

void Handle_OTAScreen_Touch(int x, int y)
{
	otaScreen->HandleTouch(x, y);
}