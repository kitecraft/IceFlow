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
	_profileListBoxX = _tft->width() - PROFILE_FILE_LISTBOX_W;
	_fileList = ProfileManager.GetListOfProfileFileNames();

	if (!ProfileManager.GetSavedProfile(&_currentProfile)) {
		Serial.println("Failed to load Profile");
	}
	else {
		Serial.println("Profile: ");
		Serial.println(_currentProfile.toJsonString());
	}

	_exitButton = new Button(
		ButtonDto(
			DMCoordinates(0, 0, EXIT_BUTTON_W, EXIT_BUTTON_H, EXIT_BUTTON_X, EXIT_BUTTON_Y),
			GlobalTheme,
			SMALL_FONT,
			BUTTON_COLOR),
		"Exit");
	Draw();
	
}

ProfilesScreen::~ProfilesScreen()
{
	delete(_exitButton);
}


void ProfilesScreen::UpdateScreen(int inKey, char* value)
{

}

void ProfilesScreen::UpdateScreenOnInterval()
{

}


void ProfilesScreen::HandleTouch(int x, int y)
{
	if (_exitButton->Touched(x, y)) {
		DisplayQueue.QueueScreenChange(SN_MAIN_SCREEN);
	}
}

void ProfilesScreen::Draw()
{
	_tft->fillScreen(TFT_BLACK);

	_tft->startWrite();
	DrawHeader();
	DrawProfileList();

	_exitButton->Draw(_tft);
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
		_currentProfile.name.c_str());

	_tft->pushImageDMA(HEADER_X, HEADER_Y, _headerW, HEADER_H, sprPtr);
	_tft->dmaWait();
	sprite.deleteSprite();
}

void ProfilesScreen::DrawProfileList()
{
	TFT_eSprite sprite(_tft);
	uint16_t* sprPtr = (uint16_t*)sprite.createSprite(PROFILE_FILE_LISTBOX_W, PROFILE_FILE_LISTBOX_H);

	sprite.fillSprite(TFT_BLACK);
	DrawRoundedBox(&sprite, DMCoordinates(0, 0, PROFILE_FILE_LISTBOX_W, PROFILE_FILE_LISTBOX_H, 0, 0), 5, GlobalTheme);


	sprite.setFreeFont(SMALL_FONT);
	sprite.setTextColor(TFT_GREEN, GlobalTheme.panelLightColor);
	sprite.setTextDatum(TL_DATUM);
	String msg = "Found " + String(_fileList.size()) + " files.";
	sprite.drawString(msg, 10, 10);
	sprite.drawString("Default Profile", 10, 60);

	_tft->pushImageDMA(_profileListBoxX, PROFILE_FILE_LISTBOX_Y, PROFILE_FILE_LISTBOX_W, PROFILE_FILE_LISTBOX_H, sprPtr);
	_tft->dmaWait();
	sprite.deleteSprite();
}