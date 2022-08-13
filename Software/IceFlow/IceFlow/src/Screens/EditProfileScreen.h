#pragma once
#include <Arduino.h>
#include <TFT_eSPI.h>
#include "../IceFlow_Config.h"
#include "Utilities/Screens_Config.h"
#include "../ProfileManager/ProfileManager.h"
#include "Widgets/Button.h"
#include "Widgets/TextBox.h"
#include "DialogBoxes/MessageBox.h"
#include "DialogBoxes/NumberPadDialogBox.h"
#include "DialogBoxes/KeyboardDialog.h"

#define HEADER_X 0
#define HEADER_Y 0
#define HEADER_W 320
#define HEADER_H (MEDIUM_FONT_TEXT_BOX_H + 10)

#define DEVICE_NAME_X ((HEADER_H/2) - 3)
#define DEVICE_NAME_Y 7
#define HEADER_TB_Y 5
#define PROFILE_LABEL_X_OFFSET 15
#define PROFILE_LABEL "Profile "

#define PES_BOX_W 160
#define PES_BOX_H 73
#define PES_BOX_Y_START HEADER_H

#define PES_TEXT_BOX_W 30
#define PES_TEXT_BOX_X  (PES_BOX_W - PES_TEXT_BOX_W - 16)
#define PES_TEXT_BOX_Y_OFFSET 26
#define PES_TEXT_BOX_Y_OFFSET_2 (PES_TEXT_BOX_Y_OFFSET + 23)

#define PES_COOLING_BOX_H (PES_BOX_H - SMALL_FONT_TEXT_BOX_H - 1)

#define PES_BUTTON_W 40
#define PES_BUTTON_H 21

#define BUTTON_COLUMN_A_X (PES_BOX_W + 10)
#define BUTTON_COLUMN_B_X (BUTTON_COLUMN_A_X + PES_BUTTON_W + 10)
#define BUTTON_COLUMN_C_X (BUTTON_COLUMN_B_X + PES_BUTTON_W + 10)
#define BUTTON_ROW_A_Y (PES_BOX_Y_START + (2 * PES_BOX_H) + 6)
#define BUTTON_ROW_B_Y (BUTTON_ROW_A_Y + PES_BUTTON_H + 4)

enum TextBoxName {
	PreHeatTargetTemperature = 0,
	PreHeatRampTime,
	SoakTime,
	SoakEndTemperature,
	ReflowTemperature,
	RampTime,
	ReflowSoakTime,
	ReflowMaxTemperature,
	CoolDownDegreesPerSecond,
	TB_NO_TEXTBOX
};
#define NUMBER_OF_TEXTBOXES 9

enum EPS_MESSAGE_BOX_TYPE {
	EPS_NO_MB,
	EPS_EXIT_MB,
	EPS_DELETE_MB,
	EPS_OK_MB,
	EPS_DELETE_SUCCESS_MB,
	EPS_SAVE_MB
};

class EditProfileScreen
{
private:
	TFT_eSPI* _tft;
	Profile _profile;
	Profile _profileCopy;
	Button* _saveButton = nullptr;
	Button* _saveAsButton = nullptr;
	Button* _cancelButton = nullptr;
	Button* _exitButton = nullptr;
	Button* _deleteButton = nullptr;

	bool _saveRequired = false;
	TextBox* _textBoxes[NUMBER_OF_TEXTBOXES] = { nullptr };
	TextBoxName _activeTextBox = TB_NO_TEXTBOX;

	MessageBox* _messageBox = nullptr;
	EPS_MESSAGE_BOX_TYPE _activeMessageBox = EPS_NO_MB;

	KeyboardDialog* _keyboard = nullptr;
	bool _keyboardOpen = false;
	
	NumberPadDialogBox* _numberPadDlg = nullptr;

	void UpdateTextBox(TextBoxName textBoxName, uint16_t val);

	void Draw();
	void DrawTextBoxes();
	void DrawHeader();
	void DrawButtons();

	void ProcessTouch(int x, int y);
	void HandleMessageBoxTouch(int x, int y);
	void EndMessageBox();
	bool HandleOKMessageBox(DialogButtonType buttonPressed);
	void HandleExitMessageBox(DialogButtonType buttonPressed);
	void HandleDeleteMessageBox(DialogButtonType buttonPressed);
	void HandleDeleteSuccessMessageBox(DialogButtonType buttonPressed);
	void HandleSaveProfileMessageBox(DialogButtonType buttonPressed);

	void HandleSaveAsKeyboard(int x, int y);

	void CreateTextBoxes();
	void UpdateTextbox(TextBoxName textBoxName);
	void CreateButtons();

	void OpenNumberPad(TextBoxName textBoxName);
	void CloseNumberPad();

public:
	EditProfileScreen(TFT_eSPI* tft);
	~EditProfileScreen();

	void HandleTouch(int x, int y);
};




static EditProfileScreen* editProfileScreen = nullptr;
static void Create_EditProfileScreen(TFT_eSPI* TFT)
{
	editProfileScreen = new EditProfileScreen(TFT);
}

static void Destroy_EditProfileScreen()
{
	if (editProfileScreen != nullptr)
	{
		delete(editProfileScreen);
		editProfileScreen = nullptr;
	}
}

static void HandleTouch_EditProfileScreen(int x, int y)
{
	editProfileScreen->HandleTouch(x, y);
}
