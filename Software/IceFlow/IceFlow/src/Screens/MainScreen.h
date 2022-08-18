#pragma once
#include <Arduino.h>
#include <TFT_eSPI.h>
#include "../IceFlow_Config.h"
#include "Utilities/Screens_Config.h"
#include "Panels/SideBar.h"
#include "Widgets/TextBox.h"
#include "../ProfileManager/ProfileManager.h"
#include "Panels/GraphPanel.h"
#include "DialogBoxes/TargetTemperatureDlg.h"
#include "DialogBoxes/MessageBox.h"

#define WIFI_X (320 - 4)
#define WIFI_Y 6
#define WIFI_SPOT_R 4

#define TIME_W 38
#define TIME_X (WIFI_X - TIME_W - 7)
#define TIME_Y 0
#define TIME_H 10

#define SIDEBAR_X (320 - SIDEBAR_W)
#define SIDEBAR_Y (TIME_H + 4)

#define MS_HEADER_X 0
#define MS_HEADER_Y 0
#define MS_HEADER_W_OFFSET 3
#define MS_HEADER_W (SIDEBAR_X - MS_HEADER_W_OFFSET)
#define MS_HEADER_H (MEDIUM_FONT_TEXT_BOX_H + 10)

#define MS_FOOTER_W_OFFSET 3
#define MS_FOOTER_X 0
#define MS_FOOTER_H (MEDIUM_FONT_TEXT_BOX_H + 10)
#define MS_FOOTER_Y (240 - MS_FOOTER_H)
#define MS_FOOTER_W (SIDEBAR_X - MS_FOOTER_W_OFFSET)

#define MS_TEMPERATURE_TB_W (MEDIUM_FONT_TEXT_BOX_H + 55)
#define MS_PRIMARY_TEMPERATURE_LABEL_X (MS_HEADER_H / 2)
#define MS_TEMPERATURE_LABEL_Y ((MS_FOOTER_H / 2) - 1)
#define MS_PRIMARY_TEMPERATURE_TB_X ( MS_PRIMARY_TEMPERATURE_LABEL_X + 33)

#define MS_SEC_TEMPERATURE_LABEL_X (MS_PRIMARY_TEMPERATURE_TB_X + MS_TEMPERATURE_TB_W + 5)
#define MS_SEC_TEMPERATURE_TB_X (MS_SEC_TEMPERATURE_LABEL_X + 25)

#define MS_DEVICE_NAME_X ((MS_HEADER_H/2) - 3)
#define MS_DEVICE_NAME_Y 7
#define MS_HEADER_TB_Y 5

#define MS_PROFILE_LABEL_X_OFFSET 15
#define MS_PROFILE_LABEL "Profile "

#define MS_GRAPHPANEL_X 0
#define MS_GRAPHPANEL_Y 34
#define MS_GRAPHPANEL_W 270
#define MS_GRAPHPANEL_H (240 - MS_GRAPHPANEL_Y*2)

#define MS_HEATER_ICON_X (MS_SEC_TEMPERATURE_TB_X + MS_TEMPERATURE_TB_W + 7)
#define MS_HEATER_ICON_Y (MS_FOOTER_Y + 4)
#define MS_HEATER_ICON_W 15
#define MS_HEATER_ICON_H 24

#define MANAUAL_HEAT_TARGET_TEMPERATURE_DIALOG_TITLE "Manual Heat"
#define AUTOTUNE_TARGET_TEMPERATURE_DIALOG_TITLE "Auto Tune"

enum MS_MESSAGE_BOX_TYPE {
	MS_NO_MB, 
	MS_OK_MB,
	MS_OVER_MAX_TEMP,
	MS_AUTOTUNE_MB
};

class MainScreen
{
private:
	TFT_eSPI* _tft;
	TFT_eSprite* _timeSprite = nullptr;
	SideBar* _sideBar = nullptr;
	Profile _currentProfile;

	GraphPanel* _graphPanel = nullptr;

	bool _temperatureStreamStarted = false;
	float _primaryTemperature = 20;
	float _secondaryTemperature = 20;
	float _tertiaryTemperature = 20;

	TextBox* _primaryTemperatureTB = nullptr;
	TextBox* _secondaryTemperatureTB = nullptr;

	TargetTemperatureDlg* _targetTemperatureDlg = nullptr;
	MessageBox* _messageBox = nullptr;
	MS_MESSAGE_BOX_TYPE _activeMessageBox = MS_NO_MB;

	unsigned long _nextGraphUpdate = 0;

	void DrawScreen();
	void DrawHeader();
	void DrawFooter();
	void DisplayTime();

	void UpdatePrimaryTemp(char* val);
	void UpdateSecondaryTemp(char* val);

	void ManualHeatTouched();
	bool ManualHeatDlgClosed(DialogButtonType action);

	void DrawHeatersIcon(bool status);

	void ProcessTouch(int x, int y);
	void HandleMessageBoxTouch(int x, int y);
	void EndMessageBox();
	bool HandleOKMessageBox(DialogButtonType buttonPressed);

public:
	MainScreen(TFT_eSPI* tft);
	~MainScreen();

	void UpdateScreen(int inKey, char* value);
	void UpdateScreenOnInterval();

	void HandleTouch(int x, int y);
};


static MainScreen* mainScreen = nullptr;
static void Create_MainScreen(TFT_eSPI* TFT)
{
	mainScreen = new MainScreen(TFT);
}

static void Destroy_MainScreen()
{
	if (mainScreen != nullptr)
	{
		delete(mainScreen);
		mainScreen = nullptr;
	}
}

static void Update_MainScreen(int inKey, char* value)
{
	mainScreen->UpdateScreen(inKey, value);
}

static void Update_MainScreenOnInterval()
{
	mainScreen->UpdateScreenOnInterval();
}

static void HandleTounch_MainScreen(int x, int y)
{
	mainScreen->HandleTouch(x, y);
}


