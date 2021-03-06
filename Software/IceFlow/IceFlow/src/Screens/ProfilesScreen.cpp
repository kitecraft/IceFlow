#include "ProfilesScreen.h"
#include "Widgets/Box.h"
#include "Widgets/TextBox.h"
#include "../ProfileManager/ProfileManager.h"
#include "../DisplayManager/Utilities/DisplayQueue.h"
#include "Utilities/ScreenNames.h"

ProfilesScreen::ProfilesScreen(TFT_eSPI* tft)
{
	_tft = tft;

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


	_showGridButton = new Button(
		ButtonDto(
			DMCoordinates(0, 0, SHOW_GRID_BTN_W, BUTTON_H, SHOW_GRID_BTN_X, SHOW_GRID_BTN_Y),
			GlobalTheme,
			SMALL_FONT,
			BUTTON_COLOR,
			GlobalTheme.panelLightColor),
		"Grid",
		_tft);

	_profileListPanel = new ProfileListPanel(_tft);
	_saveRequired = false;

	_graphPanel = new PS_ProfileGraphPanel(_tft);

	_tft->fillScreen(TFT_BLACK);
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
	if (_showGridButton != nullptr) {
		delete(_showGridButton);
	}
	if (_profileListPanel != nullptr) {
		delete(_profileListPanel);
	}
	if (_graphPanel != nullptr) {
		delete(_graphPanel);
	}
}


void ProfilesScreen::UpdateScreen(int inKey, char* value)
{
	Serial.printf("ProfilesScreen::UpdateScreen Shouldn't be here %i\n", inKey);
}

void ProfilesScreen::UpdateScreenOnInterval()
{

}


void ProfilesScreen::HandleTouch(int x, int y)
{
	if (_showGridButton->Touched(x, y)) {
		if (_graphPanel->GridVisible()) {
			_graphPanel->ShowGrid(false);
			_showGridButton->SetButtonColor(BUTTON_COLOR);
		}
		else {
			_graphPanel->ShowGrid(true);
			_showGridButton->SetButtonColor(GlobalTheme.panelLightColor);
		}
		_tft->startWrite();
		_showGridButton->Draw();
		_graphPanel->ReDraw();
		_tft->dmaWait();
		_tft->endWrite();
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

	String option;
	if (_profileListPanel->Touched(x, y, option)) {
		if (_currentlyLoadedProfile.filename == option) {
			return;
		}

		if (!ProfileManager.GetProfile(option, &_currentlyLoadedProfile)) {
			Serial.println("Failed to load Profile");
			return;
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
}

void ProfilesScreen::Draw()
{
	_tft->startWrite();
	DrawHeader();
	
	if (_saveRequired) {
		_saveButton->Visible(true);
		_cancelButton->Draw();
	}
	else {
		_saveButton->Visible(false);
		_exitButton->Draw();
	}

	_saveButton->Draw();
	_profileListPanel->Draw(_currentlyLoadedProfile.filename);
	_graphPanel->Draw(_currentlyLoadedProfile.filename);

	_tft->dmaWait();
	_tft->endWrite();

}

void ProfilesScreen::DrawHeader()
{
	TFT_eSprite sprite(_tft);
	uint16_t* sprPtr = (uint16_t*)sprite.createSprite(HEADER_W, HEADER_H);

	sprite.fillSprite(TFT_BLACK);
	DrawRoundedBox(&sprite, DMCoordinates(0, 0, HEADER_W, HEADER_H, 0, 0), HEADER_H / 2, GlobalTheme);

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
		_currentlyLoadedProfile.name.c_str());

	_tft->pushImageDMA(HEADER_X, HEADER_Y, HEADER_W, HEADER_H, sprPtr);
	_tft->dmaWait();
	sprite.deleteSprite();

	if (_graphPanel->GridVisible()) {
		_showGridButton->SetButtonColor(GlobalTheme.panelLightColor);
	}
	else {
		_showGridButton->SetButtonColor(BUTTON_COLOR);
	}
	_showGridButton->Draw();
}
