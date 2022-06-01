#pragma once
#include "MainScreen.h"


MainScreen* mainScreen = NULL;

void Create_MainScreen(TFT_eSPI* TFT) {
	mainScreen = new MainScreen(TFT);
}

void Destroy_MainScreen()
{
	if (mainScreen != NULL)
	{
		delete(mainScreen);
		mainScreen = NULL;
	}
}

void Update_MainScreen(DISPLAY_UPDATE_KEYS inKey, String inPayload)
{
	mainScreen->UpdateScreen(inKey, inPayload);
}

void Update_MainScreen_OnInterval()
{
	mainScreen->UpdateScreenOnInterval();
}

void Handle_MainScreen_Touch(int x, int y)
{
	mainScreen->HandleTouch(x, y);
}