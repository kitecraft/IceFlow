#include "PidEditor.h"
#include "../../Utilities/PreferencesManager.h"

PidEditor::PidEditor() : DialogBase()
{

}

PidEditor::~PidEditor()
{
	delete _closeButton;
	delete _currentPID_Kp;
	delete _currentPID_Ki;
	delete _currentPID_Kd;

	delete _cancelButton;
	delete _saveButton;

	delete _setMidPID;
	delete _setLowPID;
	delete _setHighPID;

	delete _runAutoTune;

	if (_messageBox != nullptr) {
		delete _messageBox;
	}

	if (_numberPad != nullptr) {
		delete _numberPad;
	}
}

PidEditor::PidEditor(TFT_eSPI* tft)
	:DialogBase(tft, DMCoordinates(0, 0, PID_DLG_W, PID_DLG_H, PID_DLG_X, PID_DLG_Y), GlobalTheme, PID_DLG_TITLE)
{
	_messageBox = nullptr;
	_messageBoxActive = PIDE_MB_NONE;
	_saveRequired = false;

	_pidData.GetData();
	_origKp = GetPidKP();
	_origKi = GetPidKI();
	_origKd = GetPidKD(); 
	
	_numberPad = nullptr;
	_activeTB = PIDED_NONE;

	_runAutoTune = new Button(
		ButtonDto(
			DMCoordinates(
				PID_DLG_RUN_BUTTON_X,
				PID_DLG_RUN_BUTTON_Y,
				PID_DLG_RUN_BUTTON_W,
				PID_DLG_ACTION_BUTTON_H,
				_coordinates.p_x + PID_DLG_RUN_BUTTON_X,
				_coordinates.p_y + PID_DLG_RUN_BUTTON_Y
			),
			GlobalTheme,
			MEDIUM_FONT,
			0x3dd0),
		"Run Autotune",
		_tft,
		false
	);

	_closeButton = new DialogButton(
		DialogButtonDto(
			DMCoordinates(
				PID_DLG_BTN_CLOSE_X,
				PID_DLG_BTN_Y,
				0,
				0,
				_coordinates.p_x + PID_DLG_BTN_CLOSE_X,
				_coordinates.p_y + PID_DLG_BTN_Y
			),
			GlobalTheme,
			MEDIUM_FONT,
			DB_CLOSE),
		_tft
	);

	_saveButton = new Button(
		ButtonDto(
			DMCoordinates(
				PID_DLG_SAVE_BUTTON_X,
				PID_DLG_BTN_Y,
				PID_DLG_ACTION_BUTTON_W,
				PID_DLG_ACTION_BUTTON_H,
				_coordinates.p_x + PID_DLG_SAVE_BUTTON_X,
				_coordinates.p_y + PID_DLG_BTN_Y
			),
			GlobalTheme,
			MEDIUM_FONT,
			GlobalTheme.panelLightColor),
		"Save",
		_tft,
		false
	);

	_cancelButton = new Button(
		ButtonDto(
			DMCoordinates(
				PID_DLG_CANCEL_BUTTON_X,
				PID_DLG_BTN_Y,
				PID_DLG_ACTION_BUTTON_W,
				PID_DLG_ACTION_BUTTON_H,
				_coordinates.p_x + PID_DLG_CANCEL_BUTTON_X,
				_coordinates.p_y + PID_DLG_BTN_Y
			),
			GlobalTheme,
			MEDIUM_FONT,
			GlobalTheme.panelLightColor),
		"Cancel",
		_tft,
		false
	);

	_currentPID_Kp = new TextBox(TextBoxDto(
		DMCoordinates(
			PID_DLG_KP_TEXTBOX_X,
			PID_DLG_BODY_Y_OFFSET + PID_DLG_PANEL_TEXTBOX_Y_OFFSET,
			PID_DLG_TEXTBOX_W,
			SMALL_FONT_TEXT_BOX_H,
			_coordinates.p_x + PID_DLG_KP_TEXTBOX_X,
			_coordinates.p_y + PID_DLG_BODY_Y_OFFSET + PID_DLG_PANEL_TEXTBOX_Y_OFFSET
		),
		GlobalTheme,
		SMALL_FONT,
		MR_DATUM),
		_tft
	);

	_currentPID_Ki = new TextBox(TextBoxDto(
		DMCoordinates(
			PID_DLG_KI_TEXTBOX_X,
			PID_DLG_BODY_Y_OFFSET + PID_DLG_PANEL_TEXTBOX_Y_OFFSET,
			PID_DLG_TEXTBOX_W,
			SMALL_FONT_TEXT_BOX_H,
			_coordinates.p_x + PID_DLG_KI_TEXTBOX_X,
			_coordinates.p_y + PID_DLG_BODY_Y_OFFSET + PID_DLG_PANEL_TEXTBOX_Y_OFFSET
		),
		GlobalTheme,
		SMALL_FONT,
		MR_DATUM),
		_tft);

	_currentPID_Kd = new TextBox(TextBoxDto(
		DMCoordinates(
			PID_DLG_KD_TEXTBOX_X,
			PID_DLG_BODY_Y_OFFSET + PID_DLG_PANEL_TEXTBOX_Y_OFFSET,
			PID_DLG_TEXTBOX_W,
			SMALL_FONT_TEXT_BOX_H,
			_coordinates.p_x + PID_DLG_KD_TEXTBOX_X,
			_coordinates.p_y + PID_DLG_BODY_Y_OFFSET + PID_DLG_PANEL_TEXTBOX_Y_OFFSET
		),
		GlobalTheme,
		SMALL_FONT,
		MR_DATUM),
		_tft);

	_setMidPID = new Button(
		ButtonDto(
			DMCoordinates(
				PID_DLG_PANEL_SET_BTN_X,
				PID_DLG_PANEL_Y_OFFSET_MID + PID_DLG_PANEL_SET_BUTTON_Y_OFFSET,
				PID_DLG_PANEL_SET_BTN_W,
				PID_DLG_PANEL_SET_BTN_H,
				_coordinates.p_x + PID_DLG_PANEL_SET_BTN_X,
				_coordinates.p_y + PID_DLG_PANEL_Y_OFFSET_MID + PID_DLG_PANEL_SET_BUTTON_Y_OFFSET
			),
			GlobalTheme,
			SMALL_FONT,
			GlobalTheme.panelLightColor),
		"Set",
		_tft,
		false
	);

	_setLowPID = new Button(
		ButtonDto(
			DMCoordinates(
				PID_DLG_PANEL_SET_BTN_X,
				PID_DLG_PANEL_Y_OFFSET_LOW + PID_DLG_PANEL_SET_BUTTON_Y_OFFSET,
				PID_DLG_PANEL_SET_BTN_W,
				PID_DLG_PANEL_SET_BTN_H,
				_coordinates.p_x + PID_DLG_PANEL_SET_BTN_X,
				_coordinates.p_y + PID_DLG_PANEL_Y_OFFSET_LOW + PID_DLG_PANEL_SET_BUTTON_Y_OFFSET
			),
			GlobalTheme,
			SMALL_FONT,
			GlobalTheme.panelLightColor),
		"Set",
		_tft,
		false
	);

	_setHighPID = new Button(
		ButtonDto(
			DMCoordinates(
				PID_DLG_PANEL_SET_BTN_X,
				PID_DLG_PANEL_Y_OFFSET_HIGH + PID_DLG_PANEL_SET_BUTTON_Y_OFFSET,
				PID_DLG_PANEL_SET_BTN_W,
				PID_DLG_PANEL_SET_BTN_H,
				_coordinates.p_x + PID_DLG_PANEL_SET_BTN_X,
				_coordinates.p_y + PID_DLG_PANEL_Y_OFFSET_HIGH + PID_DLG_PANEL_SET_BUTTON_Y_OFFSET
			),
			GlobalTheme,
			SMALL_FONT,
			GlobalTheme.panelLightColor),
		"Set",
		_tft,
		false
	);

	_currentPID_Kp->Draw(_sprite, String(_origKp));
	_currentPID_Ki->Draw(_sprite, String(_origKi));
	_currentPID_Kd->Draw(_sprite, String(_origKd));
	Draw();
}

void PidEditor::Draw()
{
	DrawPIDValuesPanel(PID_DLG_PANEL_Y_OFFSET_CURRENT, "Current: ", TFT_GREEN);
	DrawPIDValuesPanel(PID_DLG_PANEL_Y_OFFSET_MID, "Mid: ", 0xFFF2);
	DrawPIDValuesPanel(PID_DLG_PANEL_Y_OFFSET_LOW, "Low: ", 0xFFF2);
	DrawPIDValuesPanel(PID_DLG_PANEL_Y_OFFSET_HIGH, "High: ", 0xFFF2);

	_sprite->setFreeFont(SMALL_FONT);
	_sprite->setTextDatum(ML_DATUM);
	_sprite->setTextColor(GlobalTheme.textColor);

	_sprite->drawFloat(_pidData.midKp, 2, PID_DLG_KP_TEXTBOX_X + 2, PID_DLG_PANEL_Y_OFFSET_MID + (PID_DLG_PANEL_H / 2) - 2);
	_sprite->drawFloat(_pidData.midKi, 2, PID_DLG_KI_TEXTBOX_X + 2, PID_DLG_PANEL_Y_OFFSET_MID + (PID_DLG_PANEL_H / 2) - 2);
	_sprite->drawFloat(_pidData.midKd, 2, PID_DLG_KD_TEXTBOX_X + 2, PID_DLG_PANEL_Y_OFFSET_MID + (PID_DLG_PANEL_H / 2) - 2);

	_sprite->drawFloat(_pidData.lowKp, 2, PID_DLG_KP_TEXTBOX_X + 2, PID_DLG_PANEL_Y_OFFSET_LOW + (PID_DLG_PANEL_H / 2) - 2);
	_sprite->drawFloat(_pidData.lowKi, 2, PID_DLG_KI_TEXTBOX_X + 2, PID_DLG_PANEL_Y_OFFSET_LOW + (PID_DLG_PANEL_H / 2) - 2);
	_sprite->drawFloat(_pidData.lowKd, 2, PID_DLG_KD_TEXTBOX_X + 2, PID_DLG_PANEL_Y_OFFSET_LOW + (PID_DLG_PANEL_H / 2) - 2);

	_sprite->drawFloat(_pidData.highKp, 2, PID_DLG_KP_TEXTBOX_X + 2, PID_DLG_PANEL_Y_OFFSET_HIGH + (PID_DLG_PANEL_H / 2) - 2);
	_sprite->drawFloat(_pidData.highKi, 2, PID_DLG_KI_TEXTBOX_X + 2, PID_DLG_PANEL_Y_OFFSET_HIGH + (PID_DLG_PANEL_H / 2) - 2);
	_sprite->drawFloat(_pidData.highKd, 2, PID_DLG_KD_TEXTBOX_X + 2, PID_DLG_PANEL_Y_OFFSET_HIGH + (PID_DLG_PANEL_H / 2) - 2);

	_setMidPID->Draw(_sprite);
	_setLowPID->Draw(_sprite);
	_setHighPID->Draw(_sprite);

	_runAutoTune->Draw(_sprite);
	_closeButton->Draw(_sprite);
	ShowHideButtons();
}

void PidEditor::DrawPIDValuesPanel(int yOffset, String title, uint16_t borderColor)
{
	_sprite->drawRect(6, yOffset, PID_DLG_PANEL_W, PID_DLG_PANEL_H, borderColor);

	_sprite->setFreeFont(SMALL_FONT);
	_sprite->setTextDatum(ML_DATUM);
	_sprite->setTextColor(GlobalTheme.textColor);

	_sprite->drawString(title, PID_DLG_PANEL_LABEL_X, yOffset + (PID_DLG_PANEL_H / 2) - 2);

	_sprite->drawString("Kp: ", PID_DLG_KP_TEXTBOX_LABEL_X , yOffset + (PID_DLG_PANEL_H / 2) - 2);
	_sprite->drawString("Ki: ", PID_DLG_KI_TEXTBOX_LABEL_X, yOffset + +(PID_DLG_PANEL_H / 2) - 2);
	_sprite->drawString("Kd: ", PID_DLG_KD_TEXTBOX_LABEL_X, yOffset + +(PID_DLG_PANEL_H / 2) - 2);

	/*
	_sprite->drawSpot(PID_DLG_PANEL_HELP_X, yOffset + +(PID_DLG_PANEL_H / 2) - 2, 7, TFT_DARKGREY);	
	_sprite->setFreeFont(MEDIUM_FONT);
	_sprite->setTextColor(TFT_GREEN);
	_sprite->setTextDatum(MC_DATUM);
	_sprite->drawString("?", PID_DLG_PANEL_HELP_X, yOffset + +(PID_DLG_PANEL_H / 2) - 2);
	*/
}


DialogButtonType PidEditor::Touched(int x, int y)
{
	if (_messageBoxActive != PIDE_MB_NONE) {
		DialogButtonType ret = _messageBox->Touched(x, y);
		if (ret != DB_NONE) {
			if (ret == DB_CONTINUE) {
				if (_messageBoxActive == PIDE_MB_CLOSE) {
					ret = _closeButton->GetType();
				}
				else {
					ret = DB_CONTINUE;
				}
			}
			else {
				ret = DB_NONE;
			}
			_messageBox->Hide();
			_tft->dmaWait();
			delete _messageBox;
			_messageBox = nullptr;
			_messageBoxActive = PIDE_MB_NONE;
			return ret;
		}
	}


	// Number pad
	if (_activeTB != PIDED_NONE) {
		if (_numberPad->Touched(x, y)) {
			_numberPad->Hide();
			if (_numberPad->GetNumber() != 0) {
				switch (_activeTB) {
				case PIDED_KP:
					_currentPID_Kp->Update(_numberPad->GetNumber());
					break;
				case PIDED_KI:
					_currentPID_Ki->Update(_numberPad->GetNumber());
					break;
				case PIDED_KD:
					_currentPID_Kd->Update(_numberPad->GetNumber());
					break;
				}
			}
			_tft->dmaWait();
			delete _numberPad;
			_numberPad = nullptr;
			_activeTB = PIDED_NONE;
			Redraw();
		}
		return DB_NONE;
	}

	if (_runAutoTune->Touched(x, y)) {
		if (_saveRequired) {
			OpenSaveRequiredMessageBox(PIDE_MB_RUN);
		}
		else {
			return DB_CONTINUE;
		}
	}

	// Text Boxes
	if (_currentPID_Kp->Touched(x, y)) {
		OpenNumberPad(PIDED_KP, _currentPID_Kp->GetText().toFloat());
		return DB_NONE;
	}
	if (_currentPID_Ki->Touched(x, y)) {
		OpenNumberPad(PIDED_KI, _currentPID_Ki->GetText().toFloat());
		return DB_NONE;
	}
	if (_currentPID_Kd->Touched(x, y)) {
		OpenNumberPad(PIDED_KD, _currentPID_Kd->GetText().toFloat());
		return DB_NONE;
	}


	if (_closeButton->Touched(x, y)) {
		if (_saveRequired) {
			OpenSaveRequiredMessageBox(PIDE_MB_CLOSE);
		}
		else {
			return _closeButton->GetType();
		}
	}

	if (_cancelButton->Touched(x, y)) {
		ResetCurrentValuesToOrig();
		return DB_NONE;
	}
	if (_saveButton->Touched(x, y)) {
		SaveCurrentValues();
		return DB_NONE;
	}

	// Set buttons
	if (_setMidPID->Touched(x, y)) {
		SetCurrentValues(_pidData.midKp, _pidData.midKi, _pidData.midKd);
		return DB_NONE;
	}
	if (_setLowPID->Touched(x, y)){
		SetCurrentValues(_pidData.lowKp, _pidData.lowKi, _pidData.lowKd);
		return DB_NONE;
	}
	if (_setHighPID->Touched(x, y)) {
		SetCurrentValues(_pidData.highKp, _pidData.highKi, _pidData.highKd);
		return DB_NONE;
	}

	return DB_NONE;
}

void PidEditor::Redraw()
{
	DialogBase::Draw();
	_currentPID_Kp->Draw(_sprite, _currentPID_Kp->GetText());
	_currentPID_Ki->Draw(_sprite, _currentPID_Ki->GetText());
	_currentPID_Kd->Draw(_sprite, _currentPID_Kd->GetText());
	Draw();

	DialogBase::ReShow();
}

void PidEditor::ShowHideButtons()
{

	if (_currentPID_Kp->GetText() == String(_origKp) &&
		_currentPID_Ki->GetText() == String(_origKi) &&
		_currentPID_Kd->GetText() == String(_origKd)) {
		_saveButton->Visible(false);
		_cancelButton->Visible(false);
		_saveRequired = false;
	}
	else {
		_saveButton->Visible(true);
		_cancelButton->Visible(true);
		_saveButton->Draw(_sprite);
		_cancelButton->Draw(_sprite);
		_saveRequired = true;
	}
}

void PidEditor::ResetCurrentValuesToOrig()
{
	_currentPID_Kp->Update(String(_origKp));
	_currentPID_Ki->Update(String(_origKi));
	_currentPID_Kd->Update(String(_origKd));
	Redraw();
}

void PidEditor::SetCurrentValues(float kp, float ki, float kd)
{
	_currentPID_Kp->Update(String(kp));
	_currentPID_Ki->Update(String(ki));
	_currentPID_Kd->Update(String(kd));
	Redraw();
}

void PidEditor::SaveCurrentValues()
{
	float newKp = _currentPID_Kp->GetText().toFloat();
	float newKi = _currentPID_Ki->GetText().toFloat();
	float newKd = _currentPID_Kd->GetText().toFloat();
	SavePidKP(newKp);
	SavePidKI(newKi);
	SavePidKD(newKd);

	_origKp = GetPidKP();
	_origKi = GetPidKI();
	_origKd = GetPidKD();
	ResetCurrentValuesToOrig();
	Redraw();
}

void PidEditor::OpenNumberPad(PIDED_ACTIVE_TEXT_BOX textBox, float initialValue)
{
	_activeTB = textBox;
	String title = "";
	switch (textBox) {
	case PIDED_KP:
		title = "Kp";
		break;
	case PIDED_KI:
		title = "Ki";
		break;
	case PIDED_KD:
		title = "Kd";
		break;
	default:
		title = "Oops";
		break;
	}

	if (_numberPad == nullptr) {
		_numberPad = new NumberPadDialogBox(_tft, title, true);
		_numberPad->SetNumber(String(initialValue));

		//Hide();
		_numberPad->Show();
	}
}

void PidEditor::OpenSaveRequiredMessageBox(MESSAGE_BOX_ACTIVE_FOR forMB)
{
	_messageBox = new MessageBox(_tft, "Save Required", "Change have not been saved.\nAre you sure you want to continue?", MESSAGE_BOX_ICON_INFORMATION, MESSAGE_BOX_BUTTONS_CONTINUE_CANCEL);
	_messageBox->Show();
	_messageBoxActive = forMB;
}