#include "MainScreen.h"
#include "Arduino.h"
#include <ArduinoJson.h>

MainScreen::MainScreen(TFT_eSPI* newTFT) : ScreenBase(newTFT)
{
	_sideBar.Init(newTFT);
	_currentProfileFileName = GetSavedProfile();
	DrawScreen();
	DrawProfile();
}

MainScreen::~MainScreen()
{
}

void MainScreen::UpdateScreen(DISPLAY_UPDATE_KEYS inKey, String inPayload)
{
	// Keys for Display 1
	switch (inKey) {
		/*
	case <key>:
		runFunction();
		return;
		*/
	default:
		break;
	}
}

void MainScreen::UpdateScreenOnInterval()
{
}

void MainScreen::HandleTouch(int x, int y)
{
	MSSB_RETURN_VALUES touchedRet = _sideBar.Touched(x, y);
	switch (touchedRet) {
	case MSSB_TOUCHED:
		return;
	case MSSB_TOUCHED_AND_REFRESH_REQUIRED:
		DrawScreen();
		return;
	default:
		break;
	}
	
	if (_sideBar.IsMenuOpen())
	{
		return;
	}

}

void MainScreen::DrawScreen()
{
	TFT->fillScreen(TFT_BLACK);
	_sideBar.Draw();
}

void MainScreen::DrawProfile()
{
	String profileText;
	//g_profileManager.SaveProfileNameToPreferences("profile_1.json");
	//_currentProfileFileName = GetSavedProfile();

	if (_currentProfileFileName.isEmpty()) {
		profileText = "No Profile Loaded";
	}
	else {
	}

	Profile currentProfile;
	if (!g_profileManager.GetProfile("profile_1.json", currentProfile))
	{
		profileText = "Error loading profile";
	}
	
	profileText = currentProfile.name;

}