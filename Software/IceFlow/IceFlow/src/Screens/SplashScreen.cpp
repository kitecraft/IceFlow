#include "SplashScreen.h"
#include "ScreenUpdateKeys.h"
#include "../DisplayManager/Utilities/CommandQueue.h"

SplashScreen::SplashScreen(TFT_eSPI* tft)
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

	_spinner = new Spinner(_tft, _tft->width()/2, SPINNER_Y + _mainBoxY);
	uint32_t colors[8] = { 0xFFFF,0xE71C,0xCE79,0xA534,0x8410,0x630C,0x39C7,0x0000 };
	_spinner->SetColors(colors);

	Draw();
}

SplashScreen::~SplashScreen()
{
	delete(_header);
	delete(_networkName);
	delete(_ipAddress);
	delete(_spinner);
}

void SplashScreen::UpdateScreen(int inKey, char* value)
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
	default:
		break;
	}
	_tft->dmaWait();
	_tft->endWrite();
}

void SplashScreen::UpdateScreenOnInterval()
{
	_spinner->Update();
}

void SplashScreen::Draw()
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

	sprite.setTextDatum(MR_DATUM);
	sprite.drawString("SSID:", LABEL_X, SSID_BOX_Y + round(MEDIUM_FONT_TEXT_BOX_H/2));

	sprite.setFreeFont(SMALL_FONT);
	sprite.drawString("IP:", LABEL_X, IP_BOX_Y + round(SMALL_FONT_TEXT_BOX_H / 2));

	_networkName->Draw(&sprite, "Connecting...");
	_ipAddress->Draw(&sprite, "..waiting..");

	//Send updates to the screen here:
	_tft->fillScreen(TFT_BLACK);

	_tft->startWrite();
	_tft->pushImageDMA(_mainBoxX, _mainBoxY, BOX_WIDTH, BOX_HEIGHT, sprPtr);
	_tft->dmaWait();
	_tft->endWrite();

	//The spinner draws directly to the display so update
	//it after pushing the DMA image above.
	_spinner->Draw();

	sprite.deleteSprite();
}
