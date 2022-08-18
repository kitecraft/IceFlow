#pragma once
#include <Arduino.h>
#include <TFT_eSPI.h>
#include "../IceFlow_Config.h"
#include "Utilities/Screens_Config.h"
#include "Widgets/TextBox.h"
#include "../ProfileManager/ProfileManager.h"
#include "Panels/GraphPanel.h"

#include "Utilities/ScreenNames.h"
#include "Utilities/ScreenUpdateKeys.h"
#include "../DisplayManager/Utilities/CommandQueue.h"
#include "../Utilities/ControlCommands.h"

#define BASE_WIFI_X (320 - 4)
#define BASE_WIFI_Y 6
#define BASE_WIFI_SPOT_R 4

#define BASE_TIME_W 38
#define BASE_TIME_X (BASE_WIFI_X - BASE_TIME_W - 7)
#define BASE_TIME_Y 0
#define BASE_TIME_H 10

#define BASE_SIDEBAR_W 48
#define BASE_SIDEBAR_X (320 - BASE_SIDEBAR_W)
#define BASE_SIDEBAR_Y (BASE_TIME_H + 4)

#define BASE_HEADER_X 0
#define BASE_HEADER_Y 0
#define BASE_HEADER_W_OFFSET 3
#define BASE_HEADER_W (BASE_SIDEBAR_X - BASE_HEADER_W_OFFSET)
#define BASE_HEADER_H (MEDIUM_FONT_TEXT_BOX_H + 10)

#define BASE_FOOTER_W_OFFSET 3
#define BASE_FOOTER_X 0
#define BASE_FOOTER_H (MEDIUM_FONT_TEXT_BOX_H + 10)
#define BASE_FOOTER_Y (240 - BASE_FOOTER_H)
#define BASE_FOOTER_W (BASE_SIDEBAR_X - BASE_FOOTER_W_OFFSET)

#define BASE_TEMPERATURE_TB_W (MEDIUM_FONT_TEXT_BOX_H + 55)
#define BASE_PRIMARY_TEMPERATURE_LABEL_X (BASE_HEADER_H / 2)
#define BASE_TEMPERATURE_LABEL_Y ((BASE_FOOTER_H / 2) - 1)
#define BASE_PRIMARY_TEMPERATURE_TB_X ( BASE_PRIMARY_TEMPERATURE_LABEL_X + 33)

#define BASE_SEC_TEMPERATURE_LABEL_X (BASE_PRIMARY_TEMPERATURE_TB_X + BASE_TEMPERATURE_TB_W + 5)
#define BASE_SEC_TEMPERATURE_TB_X (BASE_SEC_TEMPERATURE_LABEL_X + 25)

#define BASE_DEVICE_NAME_X ((BASE_HEADER_H/2) - 3)
#define BASE_DEVICE_NAME_Y 7
#define BASE_HEADER_TB_Y 5

#define BASE_PROFILE_LABEL_X_OFFSET 15
#define BASE_PROFILE_LABEL "Profile "

#define BASE_GRAPHPANEL_X 0
#define BASE_GRAPHPANEL_Y 34
#define BASE_GRAPHPANEL_W 270
#define BASE_GRAPHPANEL_H (240 - BASE_GRAPHPANEL_Y*2)

#define BASE_HEATER_ICON_X (BASE_SEC_TEMPERATURE_TB_X + BASE_TEMPERATURE_TB_W + 7)
#define BASE_HEATER_ICON_Y (BASE_FOOTER_Y + 4)
#define BASE_HEATER_ICON_W 15
#define BASE_HEATER_ICON_H 24


class BaseScreen
{
protected:
	TFT_eSPI* _tft;
	GraphPanel* _graphPanel = nullptr;
	Profile _currentProfile;

	unsigned long _nextGraphUpdate = 0;
	bool _temperatureStreamStarted = false;
	float _primaryTemperature = 20;
	float _secondaryTemperature = 20;
	float _tertiaryTemperature = 20;

	TextBox* _primaryTemperatureTB = nullptr;
	TextBox* _secondaryTemperatureTB = nullptr;

	void DrawScreen();
	void DrawHeader();
	void DrawFooter();
	//void UpdateScreenOnInterval();

	void UpdatePrimaryTemp(char* val);
	void UpdateSecondaryTemp(char* val);

	void DrawHeatersIcon(bool status);
	
	bool UpdateScreen(SCREEN_UPDATE_KEYS key, char* value);

public:
	BaseScreen(TFT_eSPI* tft);
	~BaseScreen();
};

