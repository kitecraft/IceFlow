#pragma once
#include "DialogBase.h"
#include "../Widgets/TextBox.h"
#include "NumPadButton.h"

#define NP_NUMBER_BUTTON_WIDTH 30
#define NP_NUMBER_BUTTON_HEIGHT 30

#define NP_NUMBER_BOX_TB_X 0
#define NP_NUMBER_BOX_TB_Y (LARGE_FONT_TEXT_BOX_H - 3)
#define NP_NUMBER_BOX_TB_H LARGE_FONT_TEXT_BOX_H


#define NumberPad_DLG_W (NP_NUMBER_BUTTON_WIDTH * 3)
#define NumberPad_DLG_H ((NP_NUMBER_BUTTON_HEIGHT * 4)  + NP_NUMBER_BOX_TB_Y + NP_NUMBER_BOX_TB_H + 3)
#define NumberPad_DLG_X ((320 - NumberPad_DLG_W)/2)
#define NumberPad_DLG_Y 0

#define NP_NUMBER_BUTTON_Y (NP_NUMBER_BOX_TB_Y + NP_NUMBER_BOX_TB_H)

#define NP_NUMBER_OF_BUTTONS 12

#define MANAUAL_HEAT_TARGET_TEMPERATURE_DIALOG_TITLE "Manual Heat"

class NumberPadDialogBox :
    public DialogBase
{
private:
	int _number;
	TextBox* _textBox = nullptr;
	bool _textBoxSelected = true;

	NumPadButton* _buttons[NP_NUMBER_OF_BUTTONS] = { nullptr };

	void DrawButtons();
	void HandleButtonPress(NUM_PAD_BUTTON button);

	void UpdateValue(int newTemperature);
	void BackSpaceValue();
public:
    NumberPadDialogBox();
	NumberPadDialogBox(TFT_eSPI* tft, String title);
	~NumberPadDialogBox();
	void SetNumber(int number) { UpdateValue(number); }
	int GetNumber() { return _number; }

	bool Touched(int x, int y);
};

