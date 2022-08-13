#include "NumberPadDialogBox.h"

NumberPadDialogBox::NumberPadDialogBox() : DialogBase()
{

}

NumberPadDialogBox::NumberPadDialogBox(TFT_eSPI* tft, String title)
	:DialogBase(tft, DMCoordinates(0, 0, NumberPad_DLG_W, NumberPad_DLG_H, NumberPad_DLG_X, NumberPad_DLG_Y), GlobalTheme, title)
{
	_textBox = new TextBox(TextBoxDto(
		DMCoordinates(
			NP_NUMBER_BOX_TB_X,
			NP_NUMBER_BOX_TB_Y,
			NumberPad_DLG_W,
			LARGE_FONT_TEXT_BOX_H,
			_coordinates.p_x + NP_NUMBER_BOX_TB_X,
			_coordinates.p_y + NP_NUMBER_BOX_TB_Y
		),
		GlobalTheme,
		LARGE_FONT,
		MR_DATUM),
		_tft);
	UpdateValue(0);

	/*
	TextBox::DrawTextBox(_sprite,
		TextBoxDto(
			DMCoordinates(0, 0, _coordinates.w, LARGE_FONT_TEXT_BOX_H, 0, 0),
			GlobalTheme,
			LARGE_FONT,
			MC_DATUM,
			true),
		"Target");
	*/
	DrawButtons();
}

NumberPadDialogBox::~NumberPadDialogBox()
{
	if (_textBox != nullptr) {
		delete _textBox;
	}

	for (int i = 0; i < NP_NUMBER_OF_BUTTONS; i++) {
		delete _buttons[i];
	}
}


bool NumberPadDialogBox::Touched(int x, int y)
{
	if (!_visible) {
		return false;
	}

	if (_textBox->Touched(x, y)) {
		if (_textBoxSelected) {
			_textBoxSelected = false;
			_textBox->Update(_number);
		}
		else {
			_textBoxSelected = true;
			_textBox->Update(_number, true);
		}
		return false;
	}

	for (int i = 0; i < NP_NUMBER_OF_BUTTONS; i++) {
		NUM_PAD_BUTTON ret = _buttons[i]->Touched(x, y);
		switch(ret) {
		case NPB_NO_BUTTON:
			break;
		case NPB_ENTER:
			return true;
			break;
		case NPB_BSP:
			if (_number == 0) {
				return true;
			}
			BackSpaceValue();
			break;
		default:
			HandleButtonPress(ret);
			return false;
		}
	}

	return false;
}

void NumberPadDialogBox::HandleButtonPress(NUM_PAD_BUTTON button)
{
	if (_textBoxSelected || _number == 0) {
		UpdateValue((int)button);
	}
	else {
		char tmp[4];
		snprintf(tmp, 4, "%i%i", _number, (int)button);
		UpdateValue(atoi(tmp));
	}
}

void NumberPadDialogBox::BackSpaceValue()
{
	if (_textBoxSelected) {
		UpdateValue(0);
	}
	else {
		String tmp(_number);
		tmp.remove(tmp.length() - 1);
		UpdateValue(tmp.toInt());
	}
}

void NumberPadDialogBox::UpdateValue(int newValue)
{
	_number = newValue;
	if (_visible) {
		_textBox->Update(_number);
		_textBoxSelected = false;
	}
	else {
		_textBox->Draw(_sprite, _number, true);
		_textBoxSelected = true;
	}
}

void NumberPadDialogBox::DrawButtons()
{
	int currButton = 1;
	for (int r = 0; r < 3; r++) {
		for (int c = 0; c < 3; c++) {
			int y = NP_NUMBER_BUTTON_Y + (r * NP_NUMBER_BUTTON_HEIGHT);
			int x = c * NP_NUMBER_BUTTON_WIDTH;
			_buttons[currButton] = new NumPadButton(
				_tft,
				static_cast<NUM_PAD_BUTTON>(currButton),
				DMCoordinates(x, y, NP_NUMBER_BUTTON_WIDTH, NP_NUMBER_BUTTON_HEIGHT, _coordinates.p_x + x, _coordinates.p_y + y));
			_buttons[currButton]->Draw(_sprite);
			currButton++;
		}
	}

	int y = NP_NUMBER_BUTTON_Y + (3 * NP_NUMBER_BUTTON_HEIGHT);
	int x = 0;
	_buttons[NPB_BSP] = new NumPadButton(_tft, NPB_BSP, DMCoordinates(x, y, NP_NUMBER_BUTTON_WIDTH, NP_NUMBER_BUTTON_HEIGHT, _coordinates.p_x + x, _coordinates.p_y + y));
	_buttons[NPB_BSP]->Draw(_sprite);

	x = NP_NUMBER_BUTTON_WIDTH;
	_buttons[NPB_ZERO] = new NumPadButton(_tft, NPB_ZERO, DMCoordinates(x, y, NP_NUMBER_BUTTON_WIDTH, NP_NUMBER_BUTTON_HEIGHT, _coordinates.p_x + x, _coordinates.p_y + y));
	_buttons[NPB_ZERO]->Draw(_sprite);

	x = NP_NUMBER_BUTTON_WIDTH*2;
	_buttons[NPB_ENTER] = new NumPadButton(_tft, NPB_ENTER, DMCoordinates(x, y, NP_NUMBER_BUTTON_WIDTH, NP_NUMBER_BUTTON_HEIGHT, _coordinates.p_x + x, _coordinates.p_y + y));
	_buttons[NPB_ENTER]->Draw(_sprite);
}