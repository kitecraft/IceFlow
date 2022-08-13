#include "EditProfileScreen.h"
#include "../Utilities/PreferencesManager.h"
#include "../DisplayManager/Utilities/DisplayQueue.h"
#include "Utilities/ScreenNames.h"
#include "../Utilities/MemUseage.h"
#include "Utilities/DMTheme.h"
#include "Widgets/Box.h"

EditProfileScreen::EditProfileScreen(TFT_eSPI* tft)
{
	//Serial.println("");
	//Serial.println("EditProfileScreen: Constuctor start: ");
	//PrintMemUseage();

	_tft = tft;
	if (!ProfileManager.GetSavedProfile(&_profile)) {
		Serial.println("Failed to load Profile");
		_tft->setFreeFont(SMALL_FONT);
		_tft->setTextColor(GlobalTheme.textColor);
		_tft->setTextDatum(TL_DATUM);
		_tft->drawString("Failed to load profile", 10, 10);
		return;
	}

	_profileCopy = _profile;
	CreateTextBoxes();
	CreateButtons();

	Draw();
}

EditProfileScreen::~EditProfileScreen()
{
	if (_saveButton != nullptr) {
		delete _saveButton;
	}
	if (_saveAsButton != nullptr) {
		delete _saveAsButton;
	}
	if (_cancelButton != nullptr) {
		delete _cancelButton;
	}
	if (_exitButton != nullptr) {
		delete _exitButton;
	}
	if (_deleteButton != nullptr) {
		delete _deleteButton;
	}

	if (_messageBox != nullptr) {
		delete _messageBox;
	}
	if (_numberPadDlg != nullptr) {
		delete _numberPadDlg;
	}

	for (int i = 0; i < NUMBER_OF_TEXTBOXES; i++) {
		if (_textBoxes[i] != nullptr) {
			delete _textBoxes[i];
		}
	}
}

void EditProfileScreen::HandleTouch(int x, int y)
{
	_tft->startWrite();
	ProcessTouch(x, y);
	_tft->dmaWait();
	_tft->endWrite();
}


void EditProfileScreen::ProcessTouch(int x, int y)
{
	if (_activeMessageBox != EPS_NO_MB && _messageBox->Visible()) {
		HandleMessageBoxTouch(x, y);
		return;
	}

	if (_numberPadDlg != nullptr && _numberPadDlg->Visible()) {
		if (_numberPadDlg->Touched(x, y))
		{
			CloseNumberPad();
			return;
		}
	}

	for (int i = 0; i < NUMBER_OF_TEXTBOXES; i++) {
		if (_textBoxes[i]->Touched(x, y)) {
			if (_profile.filename != PROFILE_DEFAULT_FILE) {
				OpenNumberPad(static_cast<TextBoxName>(i));
			}
			else {
				String message = "Default profile can not be edited.";
				_messageBox = new MessageBox(_tft, "No edit", message, MESSAGE_BOX_INFORMATION, MESSAGE_BOX_OK);
				_messageBox->Show();
				_activeMessageBox = EPS_NO_EDIT_MB;
			}
			return;
		}
	}
	
	if (_saveAsButton->Touched(x, y)) {
		return;
	}

	if (_deleteButton->Touched(x, y)) {
		String message = "Are you sure you want to delete profile '" + String(_profile.name) + "'?";
		_messageBox = new MessageBox(_tft, "Confirm Delete", message, MESSAGE_BOX_QUESTION, MESSAGE_BOX_CONTINUE_CANCEL);
		_messageBox->Show();
		_activeMessageBox = EPS_DELETE_MB;
		return;
	}

	if (_exitButton->Touched(x, y)) {
		if (_saveRequired) {
			String message = "Profile has not been saved.\nContinue without saving?";
			_messageBox = new MessageBox(_tft, "Unsaved Changes", message, MESSAGE_BOX_QUESTION, MESSAGE_BOX_CONTINUE_CANCEL);
			_messageBox->Show();
			_activeMessageBox = EPS_EXIT_MB;
			return;
		}
		else {
			DisplayQueue.QueueScreenChange(SN_PROFILES_SCREEN);
		}
		return;
	}

	if (_cancelButton->Touched(x, y)) {
		_profile = _profileCopy;
		Draw();
		return;
	}
}

bool EditProfileScreen::HandleMessageBoxTouch(int x, int y)
{
	DialogButtonType ret = _messageBox->Touched(x, y);
	if (ret == DB_NONE) {
		return false;
	}

	switch (_activeMessageBox) {
	case EPS_EXIT_MB:
		return HandleExitMessageBox(ret);
		break;
	case EPS_DELETE_MB:
		return HandleDeleteMessageBox(ret);
	case EPS_NO_EDIT_MB:
		return HandleNoEditMessageBox(ret);
		break;
	}
}

void EditProfileScreen::EndMessageBox()
{
	_messageBox->Hide();
	delete _messageBox;
	_messageBox = nullptr;
	_activeMessageBox = EPS_NO_MB;
}

bool EditProfileScreen::HandleExitMessageBox(DialogButtonType buttonPressed)
{
	if (buttonPressed != DB_Continue && buttonPressed != DB_Cancel) {
		return false;
	}

	if (buttonPressed == DB_Continue) {
		DisplayQueue.QueueScreenChange(SN_PROFILES_SCREEN);
	}

	EndMessageBox();
	return true;
}

bool EditProfileScreen::HandleDeleteMessageBox(DialogButtonType buttonPressed)
{
	if (buttonPressed != DB_Continue && buttonPressed != DB_Cancel) {
		return false;
	}
	EndMessageBox();
	return true;
}

bool EditProfileScreen::HandleNoEditMessageBox(DialogButtonType buttonPressed)
{
	if (buttonPressed != DB_OK) {
		return false;
	}

	EndMessageBox();
	return true;
}

void EditProfileScreen::Draw()
{
	_tft->startWrite();
	_tft->fillScreen(TFT_BLACK);

	DrawTextBoxes();
	for (int i = 0; i < NUMBER_OF_TEXTBOXES; i++) {
		UpdateTextbox(static_cast<TextBoxName>(i));
	}
	DrawButtons();
	DrawHeader();

	_tft->dmaWait();
	_tft->endWrite();
}

void EditProfileScreen::DrawTextBoxes()
{
	TFT_eSprite sprite(_tft);
	uint16_t* sprPtr = (uint16_t*)sprite.createSprite(PES_BOX_W, PES_BOX_H);
	uint16_t* buffer = (uint16_t*)ps_malloc(((PES_BOX_W * PES_BOX_H) * 2) + 1);

	String labels[NUMBER_OF_TEXTBOXES] = { "Target temperature", "Ramp time (seconds)", "Soak time (seconds)", "Soak End Temp.",
	"Reflow Temperature", "Ramp time (seconds)", "Reflow soak(seconds)", "Max. reflow temp.", "Cooling degrees/sec" };
	int currentLabel = 0;

	String boxLabels[5] = { "1: Pre Heat", "2: Soak", "3: Reflow Ramp", "4: Reflow", "5: Cooling" };
	int currentBox = 0;
	int currentTextBox = 0;

	for (int row = 0; row < 2; row++) {
		for (int column = 0; column < 2; column++) {
			sprite.fillSprite(TFT_BLACK);

			DrawRoundedBox(&sprite, DMCoordinates(0, 0, PES_BOX_W, PES_BOX_H, 0, 0), 12, GlobalTheme);
			TextBox::DrawTextBox(&sprite,
				TextBoxDto(
					DMCoordinates(
						3,
						0,
						PES_BOX_W - 6,
						MEDIUM_FONT_TEXT_BOX_H,
						0,
						0),
					GlobalTheme,
					MEDIUM_FONT,
					MC_DATUM,
					true,
					true,
					GlobalTheme.panelLightColor),
				boxLabels[currentBox].c_str());
			currentBox++;

			sprite.setFreeFont(SMALL_FONT);
			sprite.setTextColor(GlobalTheme.textColor);
			sprite.setTextDatum(MR_DATUM);


			sprite.drawString(labels[currentTextBox], PES_TEXT_BOX_X - 5, PES_TEXT_BOX_Y_OFFSET + SMALL_FONT_TEXT_BOX_H / 2);
			currentLabel++;
			if (currentTextBox < NUMBER_OF_TEXTBOXES) {
				sprite.drawString(labels[currentLabel], PES_TEXT_BOX_X - 5, PES_TEXT_BOX_Y_OFFSET_2 + (SMALL_FONT_TEXT_BOX_H / 2) - 1);
				currentLabel++;
			}

			_textBoxes[currentTextBox]->Draw(&sprite, 0);
			currentTextBox++;

			if (currentTextBox < NUMBER_OF_TEXTBOXES) {
				_textBoxes[currentTextBox]->Draw(&sprite, 0);
				currentTextBox++;
			}
			_tft->pushImageDMA(column * PES_BOX_W, PES_BOX_Y_START + (row * PES_BOX_H), PES_BOX_W, PES_BOX_H, sprPtr, buffer);
		}
		if (currentTextBox == NUMBER_OF_TEXTBOXES) {
			break;
		}
	}


	// Draw Cooling box
	sprite.deleteSprite();
	sprPtr = (uint16_t*)sprite.createSprite(PES_BOX_W, PES_COOLING_BOX_H);
	sprite.fillSprite(TFT_BLACK);

	DrawRoundedBox(&sprite, DMCoordinates(0, 0, PES_BOX_W, PES_COOLING_BOX_H, 0, 0), 12, GlobalTheme);
	TextBox::DrawTextBox(&sprite,
		TextBoxDto(
			DMCoordinates(
				3,
				0,
				PES_BOX_W - 6,
				MEDIUM_FONT_TEXT_BOX_H,
				0,
				0),
			GlobalTheme,
			MEDIUM_FONT,
			MC_DATUM,
			true,
			true,
			GlobalTheme.panelLightColor),
		boxLabels[4].c_str());

	sprite.setFreeFont(SMALL_FONT);
	sprite.setTextColor(GlobalTheme.textColor);
	sprite.setTextDatum(MR_DATUM);


	sprite.drawString(labels[CoolDownDegreesPerSecond], PES_TEXT_BOX_X - 5, PES_TEXT_BOX_Y_OFFSET + SMALL_FONT_TEXT_BOX_H / 2);
	_textBoxes[CoolDownDegreesPerSecond]->Draw(&sprite, 0);

	_tft->pushImageDMA(0, PES_BOX_Y_START + (2 * PES_BOX_H), PES_BOX_W, PES_COOLING_BOX_H, sprPtr);

	free(buffer);
	_tft->dmaWait();
	sprite.deleteSprite();
}

void EditProfileScreen::DrawButtons()
{
	_tft->fillRect(BUTTON_COLUMN_A_X, BUTTON_ROW_A_Y, PES_BUTTON_W * 2 + 5, PES_BUTTON_H * 2 + 4, TFT_BLACK);
	_saveAsButton->Draw();
	_exitButton->Draw();

	if (_profile.filename == PROFILE_DEFAULT_FILE) {
		_saveButton->Visible(false);
		_cancelButton->Visible(false);
		_deleteButton->Visible(false);
		return;
	}

	_deleteButton->Draw();

	if (_profile == _profileCopy) {
		_saveButton->Visible(false);
		_cancelButton->Visible(false);
		_saveRequired = false;
	}
	else {
		_saveButton->Visible(true);
		_cancelButton->Visible(true);
		_saveButton->Draw();
		_cancelButton->Draw();
		_saveRequired = true;
	}
}

void EditProfileScreen::DrawHeader()
{
	TFT_eSprite sprite(_tft);
	uint16_t* sprPtr = (uint16_t*)sprite.createSprite(HEADER_W, HEADER_H);

	sprite.fillSprite(TFT_BLACK);
	DrawRoundedBox(&sprite, DMCoordinates(0, 0, HEADER_W, HEADER_H, HEADER_X, HEADER_Y), HEADER_H / 2, GlobalTheme);

	sprite.setFreeFont(LARGE_FONT);
	sprite.setTextColor(GlobalTheme.textColor, GlobalTheme.panelLightColor);
	sprite.drawString(__DEVICE_NAME__, DEVICE_NAME_X, DEVICE_NAME_Y);

	int profileLableX = DEVICE_NAME_X + sprite.textWidth(__DEVICE_NAME__) + PROFILE_LABEL_X_OFFSET;

	sprite.setFreeFont(MEDIUM_FONT);
	sprite.setTextColor(TFT_GREEN, GlobalTheme.panelLightColor);
	sprite.setTextDatum(ML_DATUM);

	sprite.drawString(PROFILE_LABEL, profileLableX, (HEADER_H / 2) - 1);

	int profileX = profileLableX + sprite.textWidth(PROFILE_LABEL);
	TextBox::DrawTextBox(&sprite,
		TextBoxDto(
			DMCoordinates(
				profileX,
				HEADER_TB_Y,
				-1,
				MEDIUM_FONT_TEXT_BOX_H,
				DEVICE_NAME_X + HEADER_X,
				HEADER_TB_Y + HEADER_Y),
			GlobalTheme,
			MEDIUM_FONT,
			MC_DATUM,
			false,
			true,
			GlobalTheme.panelLightColor),
		_profile.name.c_str());

	_tft->pushImageDMA(HEADER_X, HEADER_Y, HEADER_W, HEADER_H, sprPtr);
	_tft->dmaWait();
	sprite.deleteSprite();
}

void EditProfileScreen::CreateTextBoxes()
{
	int currentTextBox = 0;
	for (int row = 0; row < 3; row++) {
		for (int column = 0; column < 2; column++) {
			if (row == 2 && column == 1) {
				break;
			}
			_textBoxes[currentTextBox] = new TextBox(TextBoxDto(
				DMCoordinates(
					PES_TEXT_BOX_X,
					PES_TEXT_BOX_Y_OFFSET,
					PES_TEXT_BOX_W,
					SMALL_FONT_TEXT_BOX_H,
					PES_TEXT_BOX_X + (column * PES_BOX_W),
					PES_BOX_Y_START + PES_TEXT_BOX_Y_OFFSET + (row * PES_BOX_H)
				),
				GlobalTheme,
				SMALL_FONT,
				MR_DATUM),
				_tft);
			currentTextBox++;

			if (currentTextBox < NUMBER_OF_TEXTBOXES) {
				_textBoxes[currentTextBox] = new TextBox(TextBoxDto(
					DMCoordinates(
						PES_TEXT_BOX_X,
						PES_TEXT_BOX_Y_OFFSET_2,
						PES_TEXT_BOX_W,
						SMALL_FONT_TEXT_BOX_H,
						PES_TEXT_BOX_X + (column * PES_BOX_W),
						PES_BOX_Y_START + PES_TEXT_BOX_Y_OFFSET_2 + (row * PES_BOX_H)
					),
					GlobalTheme,
					SMALL_FONT,
					MR_DATUM),
					_tft);
				currentTextBox++;
			}
			else {
				break;
			}
		}
		if (currentTextBox == NUMBER_OF_TEXTBOXES) {
			break;
		}
	}
}

void EditProfileScreen::UpdateTextbox(TextBoxName textBoxName)
{
	switch(textBoxName) {
	case PreHeatTargetTemperature:
		_textBoxes[PreHeatTargetTemperature]->Update(_profile.pre_heat_target_temperature);
		break;
	case PreHeatRampTime:
		_textBoxes[PreHeatRampTime]->Update(_profile.pre_heat_ramp_time);
		break;
	case SoakTime:
		_textBoxes[SoakTime]->Update(_profile.pre_heat_soak_time);
		break;
	case SoakEndTemperature:
		_textBoxes[SoakEndTemperature]->Update(_profile.pre_heat_soak_end_temperature);
		break;
	case ReflowTemperature:
		_textBoxes[ReflowTemperature]->Update(_profile.reflow_target_temperature);
		break;
	case RampTime:
		_textBoxes[RampTime]->Update(_profile.reflow_ramp_time);
		break;
	case ReflowSoakTime:
		_textBoxes[ReflowSoakTime]->Update(_profile.reflow_soak_time);
		break;
	case ReflowMaxTemperature:
		_textBoxes[ReflowMaxTemperature]->Update(_profile.reflow_max_temperature);
		break;
	case CoolDownDegreesPerSecond:
		_textBoxes[CoolDownDegreesPerSecond]->Update(_profile.cooling_ramp_down_speed);
		break;
	}
}

void EditProfileScreen::UpdateProfile(TextBoxName textBoxName, uint16_t val)
{
	switch (textBoxName) {
	case PreHeatTargetTemperature:
		_profile.pre_heat_target_temperature = val;
		break;
	case PreHeatRampTime:
		_profile.pre_heat_ramp_time = val;
		break;
	case SoakTime:
		_profile.pre_heat_soak_time = val;
		break;
	case SoakEndTemperature:
		_profile.pre_heat_soak_end_temperature = val;
		break;
	case ReflowTemperature:
		_profile.reflow_target_temperature = val;
		break;
	case RampTime:
		_profile.reflow_ramp_time = val;
		break;
	case ReflowSoakTime:
		_profile.reflow_soak_time = val;
		break;
	case ReflowMaxTemperature:
		_profile.reflow_max_temperature = val;
		break;
	case CoolDownDegreesPerSecond:
		_profile.cooling_ramp_down_speed = val;
		break;
	}
}

void EditProfileScreen::CreateButtons()
{
	_saveButton = new Button(
		ButtonDto(
			DMCoordinates(0, 0, PES_BUTTON_W, PES_BUTTON_H, BUTTON_COLUMN_A_X, BUTTON_ROW_A_Y),
			GlobalTheme,
			SMALL_FONT,
			BUTTON_COLOR),
		"Save",
		_tft);

	_saveAsButton = new Button(
		ButtonDto(
			DMCoordinates(0, 0, PES_BUTTON_W, PES_BUTTON_H, BUTTON_COLUMN_B_X, BUTTON_ROW_A_Y),
			GlobalTheme,
			SMALL_FONT,
			BUTTON_COLOR),
		"Save As",
		_tft);

	_cancelButton = new Button(
		ButtonDto(
			DMCoordinates(0, 0, PES_BUTTON_W, PES_BUTTON_H, BUTTON_COLUMN_A_X, BUTTON_ROW_B_Y),
			GlobalTheme,
			SMALL_FONT,
			BUTTON_COLOR),
		"Cancel",
		_tft);

	_exitButton = new Button(
		ButtonDto(
			DMCoordinates(0, 0, PES_BUTTON_W, PES_BUTTON_H, BUTTON_COLUMN_B_X, BUTTON_ROW_B_Y),
			GlobalTheme,
			SMALL_FONT,
			BUTTON_COLOR),
		"Exit",
		_tft);

	_deleteButton = new Button(
		ButtonDto(
			DMCoordinates(0, 0, PES_BUTTON_W, PES_BUTTON_H, BUTTON_COLUMN_C_X, BUTTON_ROW_B_Y),
			GlobalTheme,
			SMALL_FONT,
			BUTTON_COLOR),
		"Delete",
		_tft);
}

void EditProfileScreen::OpenNumberPad(TextBoxName textBoxName)
{
	if (_numberPadDlg == nullptr) {
		_activeTextBox = textBoxName;
		
		_numberPadDlg = new NumberPadDialogBox(_tft, "Target");
		_numberPadDlg->SetNumber(_textBoxes[textBoxName]->GetText().toInt());
		_numberPadDlg->Show();
	}
}

void EditProfileScreen::CloseNumberPad()
{
	_numberPadDlg->Hide();
	
	int val = _numberPadDlg->GetNumber();
	if(val != 0){
		UpdateProfile(_activeTextBox, val);
		UpdateTextbox(_activeTextBox);
		_tft->dmaWait();
	}
	
	delete(_numberPadDlg);
	_numberPadDlg = nullptr;
	_activeTextBox = TB_NO_TEXTBOX;
	DrawButtons();
}