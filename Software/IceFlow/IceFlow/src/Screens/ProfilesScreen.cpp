#include "ProfilesScreen.h"
#include "Widgets/Box.h"
#include "Widgets/TextBox.h"
#include "../ProfileManager/ProfileManager.h"
#include "../DisplayManager/Utilities/DisplayQueue.h"
#include "Utilities/ScreenNames.h"

ProfilesScreen::ProfilesScreen(TFT_eSPI* tft)
{
	_tft = tft;
	_headerW = _tft->width();

	if (!ProfileManager.GetSavedProfile(&_currentlySavedProfile)) {
		Serial.println("Failed to load Profile");
	}

	_currentlyLoadedProfile = _currentlySavedProfile;

	_exitButton = new Button(
		ButtonDto(
			DMCoordinates(0, 0, BUTTON_W, BUTTON_H, EXIT_BUTTON_X, BUTTON_Y),
			GlobalTheme,
			SMALL_FONT,
			BUTTON_COLOR),
		"Exit",
		_tft);

	_cancelButton = new Button(
		ButtonDto(
			DMCoordinates(0, 0, BUTTON_W, BUTTON_H, EXIT_BUTTON_X, BUTTON_Y),
			GlobalTheme,
			SMALL_FONT,
			BUTTON_COLOR),
		"Cancel",
		_tft);

	_saveButton = new Button(
		ButtonDto(
			DMCoordinates(0, 0, BUTTON_W, BUTTON_H, SAVE_BUTTON_X, BUTTON_Y),
			GlobalTheme,
			SMALL_FONT,
			BUTTON_COLOR),
		"Save",
		_tft);


	_profileListPanel = new ProfileListPanel(_tft);
	_saveRequired = false;
	Draw();
	
}

ProfilesScreen::~ProfilesScreen()
{
	if (_exitButton != nullptr) {
		delete(_exitButton);
	}
	if (_saveButton != nullptr) {
		delete(_saveButton);
	}
	if (_cancelButton != nullptr) {
		delete(_cancelButton);
	}
	if (_profileListPanel != nullptr) {
		delete(_profileListPanel);
	}
}


void ProfilesScreen::UpdateScreen(int inKey, char* value)
{

}

void ProfilesScreen::UpdateScreenOnInterval()
{

}


void ProfilesScreen::HandleTouch(int x, int y)
{
	String option;
	if (_profileListPanel->Touched(x, y, option)) {
		if (_currentlyLoadedProfile.filename == option) {
			return;
		}

		
		if (!ProfileManager.GetProfile(option, &_currentlyLoadedProfile)) {
			Serial.println("Failed to load Profile");
		}
		
		if (_currentlyLoadedProfile.filename != _currentlySavedProfile.filename) {
			_saveRequired = true;
		}
		else {
			_saveRequired = false;
		}
		
		Draw();
		return;
	}

	if (_saveRequired) {
		if (_saveButton->Touched(x, y)) {
			ProfileManager.SaveProfileNameToPreferences(_currentlyLoadedProfile.filename);
			_currentlySavedProfile = _currentlyLoadedProfile;
			_saveRequired = false;
			Draw();
			return;
		}
		if (_cancelButton->Touched(x, y)) {
			_currentlyLoadedProfile = _currentlySavedProfile;
			_saveRequired = false;
			Draw();
			return;
		}
	}
	else {
		if (_exitButton->Touched(x, y)) {
			DisplayQueue.QueueScreenChange(SN_MAIN_SCREEN);
		}
	}
}

void ProfilesScreen::Draw()
{
	_tft->fillScreen(TFT_BLACK);

	_tft->startWrite();
	DrawHeader();
	
	if (_saveRequired) {
		_saveButton->Draw();
		_cancelButton->Draw();
	}
	else {
		_exitButton->Draw();
	}

	_profileListPanel->Draw(_currentlyLoadedProfile.filename);
	_tft->dmaWait();
	_tft->endWrite();
}

void ProfilesScreen::DrawHeader()
{
	TFT_eSprite sprite(_tft);
	uint16_t* sprPtr = (uint16_t*)sprite.createSprite(_headerW, HEADER_H);

	sprite.fillSprite(TFT_BLACK);
	DrawRoundedBox(&sprite, DMCoordinates(0, 0, _headerW, HEADER_H, 0, 0), HEADER_H / 2, GlobalTheme);

	int deviceNameTBWidth = TextBox::DrawTextBox(&sprite,
		TextBoxDto(
			DMCoordinates(
				DEVICE_NAME_TB_X,
				HEADER_TB_Y,
				-1,
				MEDIUM_FONT_TEXT_BOX_H,
				DEVICE_NAME_TB_X + HEADER_X,
				HEADER_TB_Y + HEADER_Y),
			GlobalTheme,
			MEDIUM_FONT,
			MC_DATUM,
			false,
			true,
			GlobalTheme.panelLightColor),
		__DEVICE_NAME__);

	sprite.setFreeFont(MEDIUM_FONT);
	sprite.setTextColor(TFT_GREEN, GlobalTheme.panelLightColor);
	sprite.setTextDatum(ML_DATUM);
	int profileLableX = DEVICE_NAME_TB_X + deviceNameTBWidth + PROFILE_LABEL_X_OFFSET;
	sprite.drawString(PROFILE_LABEL, profileLableX, (HEADER_H / 2) - 1);

	int profileX = profileLableX + sprite.textWidth(PROFILE_LABEL);
	TextBox::DrawTextBox(&sprite,
		TextBoxDto(
			DMCoordinates(
				profileX,
				HEADER_TB_Y,
				-1,
				MEDIUM_FONT_TEXT_BOX_H,
				DEVICE_NAME_TB_X + HEADER_X,
				HEADER_TB_Y + HEADER_Y),
			GlobalTheme,
			MEDIUM_FONT,
			MC_DATUM,
			false,
			true,
			GlobalTheme.panelLightColor),
		_currentlyLoadedProfile.name.c_str());

	_tft->pushImageDMA(HEADER_X, HEADER_Y, _headerW, HEADER_H, sprPtr);
	_tft->dmaWait();
	sprite.deleteSprite();
}
