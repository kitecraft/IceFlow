#include "OTAScreen.h"
#include "ScreenUpdateKeys.h"

OTAScreen::OTAScreen(TFT_eSPI* tft)
{
	_tft = tft;
	_mainBoxX = round((_tft->width() - BOX_WIDTH) / 2);
	_mainBoxY = round((_tft->height() - BOX_HEIGHT) / 2);


	_header = new TextBox(TextBoxDto(
		DMCoordinates(0, 0, BOX_WIDTH, LARGE_FONT_TEXT_BOX_H, _mainBoxX, _mainBoxY),
		GlobalTheme,
		LARGE_FONT,
		MC_DATUM,
		true),
		_tft);


	// This box will contains the network SSID value.
	// The text is using medium sized font and is aligned
	// Left Aligned in the box
	_networkName = new TextBox(TextBoxDto(
		DMCoordinates(TB_X, SSID_BOX_Y, TB_WIDTH, MEDIUM_FONT_TEXT_BOX_H, TB_X + _mainBoxX, SSID_BOX_Y + _mainBoxY),
		GlobalTheme,
		MEDIUM_FONT,
		ML_DATUM),
		_tft);

	// This box will contains the local ip address.
	// The text is using small sized font and is center
	// aligned both vertically and horizontally
	_ipAddress = new TextBox(TextBoxDto(
		DMCoordinates(TB_X, IP_BOX_Y, TB_WIDTH, SMALL_FONT_TEXT_BOX_H, TB_X + _mainBoxX, IP_BOX_Y + _mainBoxY),
		GlobalTheme,
		SMALL_FONT,
		ML_DATUM),
		_tft);

	_progressBar = new ProgressBar(ProgressBarDto(
		DMCoordinates(PROGRESSBAR_X, PROGRESSBAR_Y, PROGRESSBAR_WIDTH, MEDIUM_FONT_TEXT_BOX_H, PROGRESSBAR_X + _mainBoxX, PROGRESSBAR_Y + _mainBoxY),
		GlobalTheme,
		MEDIUM_FONT,
		TFT_BLUE,
		TFT_GREEN),
		_tft);
	
	Draw();
}

OTAScreen::~OTAScreen()
{
	delete(_header);
	delete(_networkName);
	delete(_ipAddress);
}


void OTAScreen::UpdateScreen(int inKey, char* value)
{
	_tft->startWrite();
	SCREEN_UPDATE_KEYS key = static_cast<SCREEN_UPDATE_KEYS>(inKey);
	switch (key) {
	case suk_Device_Name:
		_header->Update(value);
		break;
	case suk_Network_Name:
		_networkName->Update(value);
		break;
	case suk_Local_IP_Address:
		_ipAddress->Update(value);
		break;
	case suk_OTA_Progress:
		_progressBar->Update(atoi(value));
		break;
	case suk_OTA_Complete:
		OTAComplete();
		break;
	default:
		break;
	}
	_tft->dmaWait();
	_tft->endWrite();
}

void OTAScreen::Draw()
{
	TFT_eSprite sprite = TFT_eSprite(_tft);
	uint16_t* sprPtr = (uint16_t*)sprite.createSprite(BOX_WIDTH, BOX_HEIGHT);

	//Start with every pixel in the sprite set to black
	sprite.fillSprite(TFT_BLACK);

	//Draw a rounded box to fill the entire sprite
	DrawRoundedBox(
		&sprite,
		DMCoordinates(0, 0, BOX_WIDTH, BOX_HEIGHT),
		15,
		GlobalTheme);

	_header->Draw(&sprite, "Welcome");

	sprite.setTextColor(GlobalTheme.textColor, GlobalTheme.panelLightColor);
	sprite.setFreeFont(MEDIUM_FONT);
	sprite.setTextDatum(MC_DATUM);
	sprite.drawString("Network info", BOX_WIDTH / 2, LABEL_Y);
	sprite.drawString("Firmware is being updated.", BOX_WIDTH / 2, PROGRESSBAR_TEXT_Y);

	sprite.setTextDatum(MR_DATUM);
	sprite.drawString("SSID:", LABEL_X, SSID_BOX_Y + round(MEDIUM_FONT_TEXT_BOX_H / 2));


	sprite.setFreeFont(SMALL_FONT);
	sprite.drawString("IP:", LABEL_X, IP_BOX_Y + round(SMALL_FONT_TEXT_BOX_H / 2));

	_networkName->Draw(&sprite, "Connecting...");
	_ipAddress->Draw(&sprite, "..waiting..");
	_progressBar->Draw(&sprite);

	//Send updates to the screen here:
	_tft->fillScreen(TFT_BLACK);

	_tft->startWrite();
	_tft->pushImageDMA(_mainBoxX, _mainBoxY, BOX_WIDTH, BOX_HEIGHT, sprPtr);
	_tft->dmaWait();
	_tft->endWrite();

	sprite.deleteSprite();
}

void OTAScreen::OTAComplete()
{
	TFT_eSprite sprite = TFT_eSprite(_tft);
	uint16_t* sprPtr = (uint16_t*)sprite.createSprite(BOX_WIDTH-6, COMPLETE_TEXT_HEIGHT);

	//Start with every pixel in the sprite set to black
	sprite.fillSprite(GlobalTheme.panelLightColor);
	sprite.setFreeFont(MEDIUM_FONT);
	sprite.setTextColor(GlobalTheme.textColor, GlobalTheme.panelLightColor);
	sprite.setTextDatum(MC_DATUM);

	sprite.drawString("Update complete. Restarting...", BOX_WIDTH / 2, COMPLETE_TEXT_HEIGHT /2);

	_tft->dmaWait();
	_tft->pushImageDMA(_mainBoxX+3, PROGRESSBAR_TEXT_Y + _mainBoxY - 7, BOX_WIDTH-6, COMPLETE_TEXT_HEIGHT, sprPtr);

	sprite.deleteSprite();
}