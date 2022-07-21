#pragma once
#include <Arduino.h>
#include "../../DisplayManager/DisplayManager.h"
#include "../../DisplayManager/Utilities/ScreenContainer.h"
#include "ScreenNames.h"

#include "../OTAScreen.h"
#include "../MainScreen.h"
static void LoadScreensIntoDM()
{
	Display.AddScreen(
		SN_MAIN_SCREEN,
		ScreenContainer(
			Create_MainScreen,
			Destroy_MainScreen,
			Update_MainScreen,
			Update_MainScreenOnInterval,
			HandleTounch_MainScreen
		)
	);

	Display.AddScreen(
		SN_OTA_SCREEN,
		ScreenContainer(
			Create_OTAScreen,
			Destroy_OTAScreen,
			Update_OTAScreen,
			nullptr,
			nullptr
		)
	);
}
