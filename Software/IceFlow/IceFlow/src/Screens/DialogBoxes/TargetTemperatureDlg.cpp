#include "TargetTemperatureDlg.h"


TargetTemperatureDlg::TargetTemperatureDlg() : DialogBase()
{

}

TargetTemperatureDlg::TargetTemperatureDlg(TFT_eSPI* tft, String title)
	:DialogBase(tft, DMCoordinates(0, 0, MANUAL_HEAT_DLG_W, MANUAL_HEAT_DLG_H, MANUAL_HEAT_DLG_X, MANUAL_HEAT_DLG_Y), GlobalTheme, title)
{
	_numberPadDlg = nullptr;

	_continueButton = new DialogButton(
		DialogButtonDto(
			DMCoordinates(MANAUAL_HEAT_CONTINUE_BTN_X, MANAUAL_HEAT_BTN_Y, 0, 0, _coordinates.p_x + MANAUAL_HEAT_CONTINUE_BTN_X, _coordinates.p_y + MANAUAL_HEAT_BTN_Y),
			GlobalTheme,
			MEDIUM_FONT,
			DB_CONTINUE),
		_tft
	);

	_cancelButton = new DialogButton(
		DialogButtonDto(
			DMCoordinates(MANAUAL_HEAT_CANCEL_BTN_X, MANAUAL_HEAT_BTN_Y, 0, 0, _coordinates.p_x + MANAUAL_HEAT_CANCEL_BTN_X, _coordinates.p_y + MANAUAL_HEAT_BTN_Y),
			GlobalTheme,
			MEDIUM_FONT,
			DB_CANCEL),
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
		

	_textBox->Draw(_sprite, _targetTemperature);

	_sprite->setTextColor(_theme.textColor);
	_sprite->setFreeFont(MEDIUM_FONT);
	_sprite->setTextDatum(TC_DATUM);
	_sprite->drawString("Set the target temperture", MANUAL_HEAT_DLG_W/2, MANAUAL_HEAT_TEXT_Y);

	_continueButton->Draw(_sprite);
	_cancelButton->Draw(_sprite);
}

TargetTemperatureDlg::~TargetTemperatureDlg()
{
	delete _textBox;
	delete _continueButton;
	delete _cancelButton;

	if (_numberPadDlg != nullptr) {
		delete _numberPadDlg;
	}
}

DialogButtonType TargetTemperatureDlg::Touched(int x, int y)
{
	if (_numberPadDlg != nullptr && _numberPadDlg->Visible()) {
		if (_numberPadDlg->Touched(x, y))
		{
			CloseNumberPad();
		}
	} else if (_continueButton->Touched(x, y)) {
		return DB_CONTINUE;
	}
	else if (_cancelButton->Touched(x, y)) {
		return DB_CANCEL;
	}
	else if (_textBox->Touched(x, y)) {
		OpenNumberPad();
	}
	
	return DB_NONE;
}

void TargetTemperatureDlg::UpdateValue(int newTemperature)
{
	if (newTemperature == 0) {
		return;
	}

	_targetTemperature = newTemperature;
	if (_visible) {
		_textBox->Update(_targetTemperature);
	}
	else {
		_textBox->Draw(_sprite, _targetTemperature);
	}
}

void TargetTemperatureDlg::OpenNumberPad()
{
	if (_numberPadDlg == nullptr) {

		_numberPadDlg = new NumberPadDialogBox(_tft, "Target");
		_numberPadDlg->SetNumber(_targetTemperature);

		//Hide();
		_numberPadDlg->Show();
	}
}

void TargetTemperatureDlg::CloseNumberPad()
{
	_numberPadDlg->Hide();
	UpdateValue(_numberPadDlg->GetNumber());
	//Show();

	_tft->dmaWait();
	delete(_numberPadDlg);
	_numberPadDlg = nullptr;
}