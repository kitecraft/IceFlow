#pragma once
#include <Arduino.h>
#include "../DisplayManager/DisplayManager.h"
#include "../DisplayManager/Utilities/ScreenContainer.h"
#include "ScreenNames.h"

#include "SplashScreen.h"
#include "OTAScreen.h"

static void LoadScreensIntoDM()
{
	Display.AddScreen(
		SN_SPLASH_SCREEN, 
		ScreenContainer(
			Create_SplashScreen,
			Destroy_SplashScreen,
			Update_SplashScreen,
			Update_SplashScreen_OnInterval
		)
	);

	Display.AddScreen(
		SN_OTA_SCREEN,
		ScreenContainer(
			Create_OTAScreen,
			Destroy_OTAScreen,
			Update_OTAScreen,
			nullptr
		)
	);
}
