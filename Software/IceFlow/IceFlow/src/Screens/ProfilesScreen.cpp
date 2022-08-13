#include "ProfilesScreen.h"
#include "../ProfileManager/ProfileManager.h"
#include "../DisplayManager/Utilities/DisplayQueue.h"
#include "Utilities/ScreenNames.h"
#include "../Utilities/MemUseage.h"

ProfilesScreen::ProfilesScreen(TFT_eSPI* tft)
{
	//Serial.println("");
	//Serial.println("ProfilesScreen: Constuctor start: ");
	//PrintMemUseage();

	_tft = tft;

	if (!ProfileManager.GetSavedProfile(&_currentlySavedProfile)) {
		Serial.println("Failed to load Profile");
		_tft->setFreeFont(SMALL_FONT);
		_tft->setTextColor(GlobalTheme.textColor);
		_tft->setTextDatum(TL_DATUM);
		_tft->drawString("Failed to load profile", 10, 10);
		return;
	}

	_currentlyLoadedProfile = _currentlySavedProfile;

	_exitButton = new Button(
		ButtonDto(
			DMCoordinates(0, 0, BUTTON_W, BUTTON_H, EXIT_EDIT_BUTTON_X, BUTTON_Y),
			GlobalTheme,
			SMALL_FONT,
			BUTTON_COLOR),
		"Exit",
		_tft);

	_editButton = new Button(
			ButtonDto(
				DMCoordinates(0, 0, BUTTON_W, BUTTON_H, SAVE_SET_BUTTON_X, BUTTON_Y),
				GlobalTheme,
				SMALL_FONT,
				BUTTON_COLOR),
			"Edit",
			_tft);

	_cancelButton = new Button(
		ButtonDto(
			DMCoordinates(0, 0, BUTTON_W, BUTTON_H, EXIT_EDIT_BUTTON_X, BUTTON_Y),
			GlobalTheme,
			SMALL_FONT,
			BUTTON_COLOR),
		"Cancel",
		_tft);

	_setButton = new Button(
		ButtonDto(
			DMCoordinates(0, 0, BUTTON_W, BUTTON_H, SAVE_SET_BUTTON_X, BUTTON_Y),
			GlobalTheme,
			SMALL_FONT,
			BUTTON_COLOR),
		"Set",
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
	_setRequired = false;

	_graphPanel = new PS_ProfileGraphPanel(_tft);

	_tft->fillScreen(TFT_BLACK);
	Draw();
	
}

ProfilesScreen::~ProfilesScreen()
{
	if (_exitButton != nullptr) {
		delete(_exitButton);
	}
	if (_editButton != nullptr) {
		delete(_editButton);
	}
	if (_setButton != nullptr) {
		delete(_setButton);
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
	//Serial.println("");
	//Serial.println("ProfilesScreen: Destuctor end: ");
	//PrintMemUseage();
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
	_tft->startWrite();
	ProcessTouch(x, y);
	_tft->dmaWait();
	_tft->endWrite();
}

void ProfilesScreen::ProcessTouch(int x, int y) 
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
		_showGridButton->Draw();
		_graphPanel->ReDraw();
		return;
	}

	if (_setRequired) {
		if (_setButton->Touched(x, y)) {
			ProfileManager.SaveProfileNameToPreferences(_currentlyLoadedProfile.filename);
			_currentlySavedProfile = _currentlyLoadedProfile;
			_setRequired = false;
			Draw();
			return;
		}
		if (_cancelButton->Touched(x, y)) {
			_currentlyLoadedProfile = _currentlySavedProfile;
			_setRequired = false;
			Draw();
			return;
		}
	}
	else {
		if (_exitButton->Touched(x, y)) {
			DisplayQueue.QueueScreenChange(SN_MAIN_SCREEN);
		}
		if (_editButton->Touched(x, y)) {
			DisplayQueue.QueueScreenChange(SN_EDIT_PROFILES_SCREEN);
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
			_setRequired = true;
		}
		else {
			_setRequired = false;
		}

		Draw();
		return;
	}

}

void ProfilesScreen::Draw()
{
	_tft->startWrite();
	DrawHeader();
	
	if (_setRequired) {
		_setButton->Visible(true);
		_cancelButton->Visible(true);
		_editButton->Visible(false);
		_exitButton->Visible(false);
	}
	else {
		_setButton->Visible(false);
		_cancelButton->Visible(false);
		_editButton->Visible(true);
		_exitButton->Visible(true);
	}

	_setButton->Draw();
	_editButton->Draw();
	_cancelButton->Draw();
	_exitButton->Draw();
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
