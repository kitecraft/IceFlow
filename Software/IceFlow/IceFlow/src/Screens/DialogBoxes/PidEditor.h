#pragma once
#include "DialogBase.h"
#include "DialogButton.h"
#include "../Widgets/TextBox.h"
#include "../../OvenController/PidData.h"
#include "../Widgets/Button.h"
#include "NumberPadDialogBox.h"
#include "MessageBox.h"

#define PID_DLG_TITLE "PID Editor"

#define PID_DLG_X 20
#define PID_DLG_Y 20
#define PID_DLG_W 280
#define PID_DLG_H 180

#define PID_DLG_BODY_Y_OFFSET LARGE_FONT_TEXT_BOX_H


#define PID_DLG_TEXTBOX_W 35

#define PID_DLG_PANEL_LABEL_X 8

#define PID_DLG_KP_TEXTBOX_LABEL_X 50
#define PID_DLG_KP_TEXTBOX_X 66

#define PID_DLG_KI_TEXTBOX_LABEL_X 115
#define PID_DLG_KI_TEXTBOX_X 131

#define PID_DLG_KD_TEXTBOX_LABEL_X 180
#define PID_DLG_KD_TEXTBOX_X 196

#define PID_DLG_PANEL_HELP_X 263
#define PID_DLG_PANEL_SET_BTN_X 235
#define PID_DLG_PANEL_SET_BTN_W 30
#define PID_DLG_PANEL_SET_BTN_H 15

#define PID_DLG_PANEL_W (PID_DLG_W - 12)
#define PID_DLG_PANEL_H 26

#define PID_DLG_PANEL_SPACING 3
#define PID_DLG_PANEL_Y_OFFSET (PID_DLG_PANEL_H + PID_DLG_PANEL_SPACING)

#define PID_DLG_PANEL_Y_OFFSET_CURRENT (PID_DLG_BODY_Y_OFFSET + 3)
#define PID_DLG_PANEL_Y_OFFSET_MID (PID_DLG_PANEL_Y_OFFSET_CURRENT + PID_DLG_PANEL_Y_OFFSET)
#define PID_DLG_PANEL_Y_OFFSET_LOW (PID_DLG_PANEL_Y_OFFSET_CURRENT + (PID_DLG_PANEL_Y_OFFSET*2))
#define PID_DLG_PANEL_Y_OFFSET_HIGH (PID_DLG_PANEL_Y_OFFSET_CURRENT + (PID_DLG_PANEL_Y_OFFSET*3))
#define PID_DLG_PANEL_SET_BUTTON_Y_OFFSET 5
#define PID_DLG_PANEL_TEXTBOX_Y_OFFSET 7

#define PID_DLG_ACTION_BUTTON_W 65
#define PID_DLG_ACTION_BUTTON_H 23
#define PID_DLG_SAVE_BUTTON_X 8
#define PID_DLG_CANCEL_BUTTON_X 76
#define PID_DLG_BTN_CLOSE_X (PID_DLG_W - (DIALOG_BUTTON_WIDTH + 5))
#define PID_DLG_BTN_Y (PID_DLG_H - (DIALOG_BUTTON_HEIGHT + 6))

#define PID_DLG_RUN_BUTTON_W 94
#define PID_DLG_RUN_BUTTON_X (PID_DLG_W - (PID_DLG_RUN_BUTTON_W + 4))
#define PID_DLG_RUN_BUTTON_Y 2

enum PIDED_ACTIVE_TEXT_BOX {
	PIDED_NONE,
	PIDED_KP,
	PIDED_KI,
	PIDED_KD
};

enum MESSAGE_BOX_ACTIVE_FOR {
	PIDE_MB_NONE = 0,
	PIDE_MB_CLOSE,
	PIDE_MB_RUN
};

class PidEditor :
    public DialogBase
{
private:
	float _origKp = 0.0;
	float _origKi = 0.0;
	float _origKd = 0.0;
	PidData _pidData;

	DialogButton* _closeButton = nullptr;
	TextBox* _currentPID_Kp = nullptr;
	TextBox* _currentPID_Ki = nullptr;
	TextBox* _currentPID_Kd = nullptr;

	Button* _setMidPID = nullptr;
	Button* _setLowPID = nullptr;
	Button* _setHighPID = nullptr;

	Button* _cancelButton = nullptr;
	Button* _saveButton = nullptr;

	Button* _runAutoTune = nullptr;

	bool _saveRequired = false;
	MessageBox* _messageBox = nullptr;
	MESSAGE_BOX_ACTIVE_FOR _messageBoxActive = PIDE_MB_NONE;

	NumberPadDialogBox* _numberPad = nullptr;
	PIDED_ACTIVE_TEXT_BOX _activeTB = PIDED_NONE;

	void DrawPIDValuesPanel(int yOffset, String title, uint16_t borderColor);
	void ShowHideButtons();
	void Draw();

	void ResetCurrentValuesToOrig();
	void SetCurrentValues(float kp, float ki, float kd);
	void SaveCurrentValues();

	void OpenNumberPad(PIDED_ACTIVE_TEXT_BOX textBox, float initialValue);

	void Redraw();
	void OpenSaveRequiredMessageBox(MESSAGE_BOX_ACTIVE_FOR forMB);

public:
	PidEditor();
	PidEditor(TFT_eSPI* tft);
	~PidEditor();

	DialogButtonType Touched(int x, int y);
};

