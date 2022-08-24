#pragma once
#include <Arduino.h>
#include "../../DisplayManager/DisplayManager.h"
#include "../../DisplayManager/Utilities/ScreenContainer.h"
#include "ScreenNames.h"

#include "../OTAScreen.h"
#include "../MainScreen.h"
#include "../ProfilesScreen.h"
#include "../EditProfileScreen.h"
#include "../ReflowScreen.h"
#include "../InfoScreen.h"

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

	Display.AddScreen(
		SN_PROFILES_SCREEN,
		ScreenContainer(
			Create_ProfilesScreen,
			Destroy_ProfilesScreen,
			Update_ProfilesScreen,
			nullptr,
			HandleTouch_ProfilesScreen
		)
	);

	Display.AddScreen(
		SN_EDIT_PROFILES_SCREEN,
		ScreenContainer(
			Create_EditProfileScreen,
			Destroy_EditProfileScreen,
			nullptr,
			nullptr,
			HandleTouch_EditProfileScreen
		)
	);

	Display.AddScreen(
		SN_REFLOW_SCREEN,
		ScreenContainer(
			Create_ReflowScreen,
			Destroy_ReflowScreen,
			Update_ReflowScreen,
			Update_ReflowScreen_OnInterval,
			HandleTouch_ReflowScreen
		)
	);

	Display.AddScreen(
		SN_INFO_SCREEN,
		ScreenContainer(
			Create_InfoScreen,
			Destroy_InfoScreen,
			Update_InfoScreen,
			NULL,
			HandleTouch_InfoScreen
		)
	);
}
