#pragma once
#include <Arduino.h>
#include "BaseScreen.h"
#include "Panels/SideBar.h"
#include "DialogBoxes/TargetTemperatureDlg.h"
#include "DialogBoxes/MessageBox.h"


#define MANAUAL_HEAT_TARGET_TEMPERATURE_DIALOG_TITLE "Manual Heat"
#define AUTOTUNE_TARGET_TEMPERATURE_DIALOG_TITLE "Auto Tune"

enum MS_MESSAGE_BOX_TYPE {
	MS_NO_MB, 
	MS_OK_MB,
	MS_OVER_MAX_TEMP,
	MS_AUTOTUNE_MB,
	MS_START_REFLOW_MB,
	MS_START_REFLOW_TEMP_ERROR_MB
};

class MainScreen : BaseScreen
{
private:
	TFT_eSprite* _timeSprite = nullptr;
	SideBar* _sideBar = nullptr;

	TargetTemperatureDlg* _targetTemperatureDlg = nullptr;
	MessageBox* _messageBox = nullptr;
	MS_MESSAGE_BOX_TYPE _activeMessageBox = MS_NO_MB;

	void DrawScreen();
	void DisplayTime();

	void ReflowTouched();

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


