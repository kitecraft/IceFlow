#include "MainScreen.h"
#include "Arduino.h"
#include <ArduinoJson.h>

MainScreen::MainScreen(TFT_eSPI* newTFT) : ScreenBase(newTFT)
{
	_msmHeader.Init(newTFT);
	_sideBar.Init(newTFT);
	_temperaturePanel.Init(newTFT);

	LoadProfile();
	DrawScreen();
	//DrawProfile();
}

MainScreen::~MainScreen()
{
}

void MainScreen::UpdateScreen(DISPLAY_UPDATE_KEYS inKey, String inPayload)
{
	// Keys for Display 1
	switch (inKey) {
	case DISPLAY_UPDATE_KEY_PRIMARY_TEMPERATURE:
		_temperaturePanel.UpdatePrimaryTemperature(inPayload.toDouble());
		return;
	case DISPLAY_UPDATE_KEY_SECONDARY_TEMPERATURE:
		_temperaturePanel.UpdateSecondaryTemperature(inPayload.toDouble());
		return;
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
	case MSSB_START_MANUAL_PREHEAT:
		g_commandQueue.AddItemToQueue(STARSIDE_CMD_START_MANUAL_PREHEAT, "150");
		break;
	case MSSB_START_REFLOW:
		if (!_currentProfileFileName.isEmpty()) {
			g_commandQueue.AddItemToQueue(STARSIDE_CMD_START_REFLOW, _currentProfileFileName);
		}
	default:
		break;
	}
	
	if (_sideBar.IsMenuOpen())
	{
		return;
	}

}

void MainScreen::LoadProfile()
{
	_currentProfileFileName = GetSavedProfile();
	if (!_currentProfileFileName.isEmpty()) {
		if (!g_profileManager.GetProfile(_currentProfileFileName, &_currentProfile)) {
			_currentProfile.name = String(PROFILE_ERROR_NAME_TEXT);
		}
	}
}

void MainScreen::DrawScreen()
{
	TFT->fillRectHGradient(0, 0, TFT_DISPLAY_WIDTH, TFT_DISPLAY_HEIGHT, TFT_WHITE, TFT_BLACK);
	_msmHeader.DrawPanel(_currentProfile.name);
	_sideBar.DrawPanel();
	_temperaturePanel.DrawPanel();
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
	if (!g_profileManager.GetProfile("profile_1.json", &currentProfile))
	{
		profileText = "Error loading profile";
	}
	
	profileText = currentProfile.name;
}