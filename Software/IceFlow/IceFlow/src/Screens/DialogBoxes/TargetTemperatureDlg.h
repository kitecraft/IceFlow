#pragma once
#include <Arduino.h>
#include "DialogBase.h"
#include "../Widgets/TextBox.h"
#include "DialogButton.h"
#include "NumberPadDialogBox.h"


#define MANUAL_HEAT_DLG_W 190
#define MANUAL_HEAT_DLG_H 140
#define MANUAL_HEAT_DLG_X ((320 - MANUAL_HEAT_DLG_W)/2)
#define MANUAL_HEAT_DLG_Y ((240 - MANUAL_HEAT_DLG_H)/2)

#define MANUAL_HEAT_TB_W 40
#define MANUAL_HEAT_TB_X (MANUAL_HEAT_DLG_W/2 - MANUAL_HEAT_TB_W/2)
#define MANUAL_HEAT_TB_Y 43
#define MANUAL_HEAT_TB_H MEDIUM_FONT_TEXT_BOX_H

#define MANAUAL_HEAT_TEXT_Y (MANUAL_HEAT_TB_Y + MEDIUM_FONT_TEXT_BOX_H + 8)

#define MANAUAL_HEAT_BTN_Y (MANUAL_HEAT_DLG_H - DIALOG_BUTTON_HEIGHT - 8)
#define MANAUAL_HEAT_CANCEL_BTN_X ((MANUAL_HEAT_DLG_W/2) - (DIALOG_BUTTON_WIDTH + 5))
#define MANAUAL_HEAT_CONTINUE_BTN_X ((MANUAL_HEAT_DLG_W/2) + 5)


class TargetTemperatureDlg :
	public DialogBase
{
private:
	int _targetTemperature = 0;
	TextBox* _textBox = nullptr;
	String _title = "";

	DialogButton* _continueButton = nullptr;
	DialogButton* _cancelButton = nullptr;

	NumberPadDialogBox* _numberPadDlg = nullptr;

	void OpenNumberPad();
	void CloseNumberPad();
	void UpdateValue(int newTemperature);

public:
	TargetTemperatureDlg();
	TargetTemperatureDlg(TFT_eSPI* tft, String title);
	~TargetTemperatureDlg();

	DialogButtonType Touched(int x, int y);
	int GetTargetTemperature() { return _targetTemperature; }
	void SetTargetTemperature(int newTarget) { UpdateValue(newTarget); }
};

