#pragma once
#include <Arduino.h>
#include "../IceFlow_Config.h"
#include "Widgets/Button.h"
#include "Utilities/DMTheme.h"
#include "Utilities/DMCoordinates.h"
#include "Widgets/TextBox.h"
#include "DialogBoxes/MessageBox.h"
#include "DialogBoxes/NumberPadDialogBox.h"

#define IS_HEADER_X 0
#define IS_HEADER_Y 0
#define IS_HEADER_W 320
#define IS_HEADER_H (MEDIUM_FONT_TEXT_BOX_H + 10)

#define IS_EXIT_BUTTON_W 65
#define IS_EXIT_BUTTON_H 23
#define IS_EXIT_BUTTON_X (320 - IS_EXIT_BUTTON_W)
#define IS_EXIT_BUTTON_Y (240 - IS_EXIT_BUTTON_H)

#define IS_DEVICE_NAME_X ((IS_HEADER_H/2) - 3)
#define IS_DEVICE_NAME_Y 7
#define IS_AUTHOR_NAME_X 160

#define IS_NETWORK_BOX_X 0
#define IS_NETWORK_BOX_Y IS_HEADER_H
#define IS_NETWORK_BOX_W 320
#define IS_NETWORK_BOX_H 82

#define IS_NETWORK_NAME_TB_X 130
#define IS_NETWORK_NAME_TB_Y 5
#define IS_NETWORK_IP_ADDRESS_TB_X 90
#define IS_NETWORK_IP_ADDRESS_TB_Y 29
#define IS_NETWORK_MAC_ADDRESS_TB_X 90
#define IS_NETWORK_MAC_ADDRESS_TB_Y 53
#define IS_NETWORK_CLEAR_BUTTON_X 245
#define IS_NETWORK_CLEAR_BUTTON_Y 29

#define IS_DISPLAY_BOX_X 0
#define IS_DISPLAY_BOX_Y (IS_NETWORK_BOX_Y + IS_NETWORK_BOX_H)
#define IS_DISPLAY_BOX_W 320
#define IS_DISPLAY_BOX_H 40
#define IS_DISPLAY_DELETE_CALIBRATION_X 194
#define IS_DISPLAY_DELETE_CALIBRATION_Y 22
#define IS_DISPLAY_DELETE_CALIBRATION_W 100
#define IS_DISPLAY_DELETE_CALIBRATION_H 13

#define IS_DISPLAY_MAX_TEMP_X 0
#define IS_DISPLAY_MAX_TEMP_Y (IS_DISPLAY_BOX_Y + IS_DISPLAY_BOX_H)
#define IS_DISPLAY_MAX_TEMP_W 320
#define IS_DISPLAY_MAX_TEMP_H 40 
#define IS_DISPLAY_MAX_TEMP_TB_X 240
#define IS_DISPLAY_MAX_TEMP_TB_Y 8
#define IS_DISPLAY_MAX_TEMP_TB_W 35
#define IS_DISPLAY_MAX_TEMP_TB_H MEDIUM_FONT_TEXT_BOX_H
#define IS_DISPLAY_APPLY_BUTTON_X 276
#define IS_DISPLAY_APPLY_BUTTON_Y 8
#define IS_DISPLAY_APPLY_BUTTON_W 38
#define IS_DISPLAY_APPLY_BUTTON_H 22

enum IS_MESSAGE_BOX_ACTIVE_FOR {
	IS_MBAF_NONE = 0,
	IS_MBAF_CONFRIM_CLEAR_NETWORK,
	IS_MBAF_NETWORK_CLEARED,
	IS_MBAF_CONFIRM_DELETE_CALIBRATION,
	IS_MBAF_CALIBRATION_DELETED,
	IS_MBAF_SAVED_DO_NOT_EXCEED,
};

class InfoScreen
{
private:
	TFT_eSPI* _tft;
	Button* _exitButton = nullptr;

	MessageBox* _messageBox = nullptr;
	IS_MESSAGE_BOX_ACTIVE_FOR _messageBoxActive = IS_MBAF_NONE;

	TextBox* _networkNameTB = nullptr;
	TextBox* _networkIPAddressTB = nullptr;
	TextBox* _macAddressTB = nullptr;
	Button* _clearNetworkInfoButton = nullptr;
	
	Button* _deleteTouchCalibarationButton = nullptr;

	TextBox* _maximumOvenTemperature = nullptr;
	NumberPadDialogBox* _numberPad = nullptr;
	Button* _applyNewMaximumTemperature = nullptr;

	void CreateNetworkWidgets();

	void Draw();
	void DrawHeader();
	void DrawNetwork();
	void DrawTouchCalibration();
	void DrawMaximumTemperature();
	
	void ProcessTouch(int x, int y);
	void HandleMessageBoxTouch(int x, int y);
	void EndMessageBox();
public:
	InfoScreen(TFT_eSPI* tft);
	~InfoScreen();

	void UpdateScreen(int inKey, char* value);
	//void UpdateScreenOnInterval();

	void HandleTouch(int x, int y);
};



static InfoScreen* infoScreen = nullptr;
static void Create_InfoScreen(TFT_eSPI* TFT)
{
	infoScreen = new InfoScreen(TFT);
}

static void Destroy_InfoScreen()
{
	if (infoScreen != nullptr)
	{
		delete(infoScreen);
		infoScreen = nullptr;
	}
}

static void Update_InfoScreen(int inKey, char* value)
{
	infoScreen->UpdateScreen(inKey, value);
}

/*
static void Update_InfoScreen_OnInterval()
{
	infoScreen->UpdateScreenOnInterval();
}
*/

static void HandleTouch_InfoScreen(int x, int y)
{
	infoScreen->HandleTouch(x, y);
}
