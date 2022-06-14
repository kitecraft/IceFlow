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
}

MainScreen::~MainScreen()
{
}

void MainScreen::UpdateScreen(DISPLAY_UPDATE_KEYS inKey, String inPayload)
{
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
		g_commandQueue.AddItemToQueue(
			STARSIDE_CMD_START_MANUAL_PREHEAT,
			_currentProfile.pre_heat_target_temperature == 0 ? String(GetDefaultPreHeatTemp()) : String(_currentProfile.pre_heat_target_temperature));
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
	SaveProfileName(String(PROFILE_DEFAULT_FILE));
	_currentProfileFileName = String(PROFILE_SPIFFS_FILE_DIRECTORY) + GetSavedProfile();

	if (!_currentProfileFileName.isEmpty()) {
		if (!g_profileManager.GetProfile(_currentProfileFileName, &_currentProfile)) {
			_currentProfile.name = String(PROFILE_ERROR_NAME_TEXT);
			_sideBar.SetStartIconStatus(false);
		}
		else {
			_sideBar.SetStartIconStatus(true);
		}
	}
	else {
		_sideBar.SetStartIconStatus(false);
	}
}

void MainScreen::DrawScreen()
{
	TFT->fillRectHGradient(0, 0, TFT_DISPLAY_WIDTH, TFT_DISPLAY_HEIGHT, TFT_WHITE, TFT_BLACK);
	_msmHeader.DrawPanel(_currentProfile.name);
	_sideBar.DrawPanel();
	_temperaturePanel.DrawPanel();
}
