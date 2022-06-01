#pragma once
#include "SplashScreen.h"


SplashScreen* splashScreen = NULL;

void Create_SplashScreen(TFT_eSPI* TFT) {
	splashScreen = new SplashScreen(TFT);
}

void Destroy_SplashScreen()
{
	if (splashScreen != NULL)
	{
		delete(splashScreen);
		splashScreen = NULL;
	}
}

void Update_SplashScreen(DISPLAY_UPDATE_KEYS inKey, String inPayload)
{
	splashScreen->UpdateScreen(inKey, inPayload);
}

void Update_SplashScreen_OnInterval()
{
	splashScreen->UpdateScreenOnInterval();
}