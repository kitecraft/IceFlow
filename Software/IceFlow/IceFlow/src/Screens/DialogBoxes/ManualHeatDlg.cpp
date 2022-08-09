#include "ManualHeatDlg.h"


ManualHeatDlg::ManualHeatDlg() : DialogBase()
{

}

ManualHeatDlg::ManualHeatDlg(TFT_eSPI* tft) 
	:DialogBase(tft, DMCoordinates(0, 0, MANUAL_HEAT_DLG_W, MANUAL_HEAT_DLG_H, MANUAL_HEAT_DLG_X, MANUAL_HEAT_DLG_Y), GlobalTheme, MEDIUM_FONT)
{
	_continueButton = new DialogButton(
		DialogButtonDto(
			DMCoordinates(MANAUAL_HEAT_CONTINUE_BTN_X, MANAUAL_HEAT_BTN_Y, 0, 0, _coordinates.p_x + MANAUAL_HEAT_CONTINUE_BTN_X, _coordinates.p_y + MANAUAL_HEAT_BTN_Y),
			GlobalTheme,
			MEDIUM_FONT,
			DB_Continue),
		_tft
	);

	_cancelButton = new DialogButton(
		DialogButtonDto(
			DMCoordinates(MANAUAL_HEAT_CANCEL_BTN_X, MANAUAL_HEAT_BTN_Y, 0, 0, _coordinates.p_x + MANAUAL_HEAT_CANCEL_BTN_X, _coordinates.p_y + MANAUAL_HEAT_BTN_Y),
			GlobalTheme,
			MEDIUM_FONT,
			DB_Cancel),
		_tft
	);


	_textBox = new TextBox(TextBoxDto(
		DMCoordinates(
			MANUAL_HEAT_TB_X,
			MANUAL_HEAT_TB_Y,
			MANUAL_HEAT_TB_W,
			MANUAL_HEAT_TB_H,
			_coordinates.p_x + MANUAL_HEAT_TB_X,
			_coordinates.p_y + MANUAL_HEAT_TB_Y
		),
		GlobalTheme,
		MEDIUM_FONT,
		MR_DATUM),
		_tft);
		
	TextBox::DrawTextBox(_sprite, 
		TextBoxDto(
			DMCoordinates(0, 0, _coordinates.w, LARGE_FONT_TEXT_BOX_H,0,0),
		GlobalTheme,
		LARGE_FONT,
		MC_DATUM,
		true), 
		"Manual Heat");

	_textBox->Draw(_sprite, _targetTemperature);

	_sprite->setTextColor(_theme.textColor);
	_sprite->setFreeFont(MEDIUM_FONT);
	_sprite->setTextDatum(TC_DATUM);
	_sprite->drawString("Set the target temperture", MANUAL_HEAT_DLG_W/2, MANAUAL_HEAT_TEXT_Y);

	_continueButton->Draw(_sprite);
	_cancelButton->Draw(_sprite);
}

ManualHeatDlg::~ManualHeatDlg()
{
	if (_textBox != nullptr) {
		delete _textBox;
	}
	if (_continueButton != nullptr) {
		delete _continueButton;
	}
	if (_cancelButton != nullptr) {
		delete _cancelButton;
	}
}

DialogButtonType ManualHeatDlg::Touched(int x, int y)
{
	if (_continueButton->Touched(x, y)) {
		return DB_Continue;
	}
	else if (_cancelButton->Touched(x, y)) {
		return DB_Cancel;
	}
	else if (_textBox->Touched(x, y)) {
		Serial.println("TEXT box was touched!");
	}
	
	return DB_NONE;
}