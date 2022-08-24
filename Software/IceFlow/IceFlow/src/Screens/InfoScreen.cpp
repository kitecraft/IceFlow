#include "InfoScreen.h"
#include "Utilities/ScreenNames.h"
#include "Utilities/ScreenUpdateKeys.h"
#include "../DisplayManager/Utilities/DisplayQueue.h"
#include "Widgets/Box.h"

InfoScreen::InfoScreen(TFT_eSPI* tft)
{
	_tft = tft;
	_exitButton = new Button(
		ButtonDto(
			DMCoordinates(
				0,
				0,
				IS_EXIT_BUTTON_W,
				IS_EXIT_BUTTON_H,
				IS_EXIT_BUTTON_X,
				IS_EXIT_BUTTON_Y
			),
			GlobalTheme,
			MEDIUM_FONT,
			GlobalTheme.panelLightColor),
		"Exit",
		_tft,
		true
	);


	Draw();
}

InfoScreen::~InfoScreen()
{
	delete _exitButton;
}

void InfoScreen::UpdateScreen(int inKey, char* value)
{

}

void InfoScreen::HandleTouch(int x, int y)
{
	_tft->startWrite();
	ProcessTouch(x, y);
	_tft->dmaWait();
	_tft->endWrite();
}

void InfoScreen::ProcessTouch(int x, int y)
{
	if (_exitButton->Touched(x, y)) {
		DisplayQueue.QueueScreenChange(SN_MAIN_SCREEN);
	}
}

void InfoScreen::Draw()
{
	_tft->fillScreen(TFT_BLACK);

	_tft->startWrite();
	DrawHeader();

	_exitButton->Draw();

	_tft->dmaWait();
	_tft->endWrite();
}

void InfoScreen::DrawHeader()
{
	TFT_eSprite sprite(_tft);
	uint16_t* sprPtr = (uint16_t*)sprite.createSprite(IS_HEADER_W, IS_HEADER_H);
	uint16_t* buffer = (uint16_t*)ps_malloc(((IS_HEADER_W * IS_HEADER_H) * 2) + 1);

	sprite.fillSprite(TFT_BLACK);
	DrawRoundedBox(&sprite, DMCoordinates(IS_HEADER_X, IS_HEADER_Y, IS_HEADER_W, IS_HEADER_H, 0, 0), IS_HEADER_H / 2, GlobalTheme);

	sprite.setFreeFont(LARGE_FONT);
	sprite.setTextColor(GlobalTheme.textColor, GlobalTheme.panelLightColor);
	String message = String(__DEVICE_NAME__) + "  v" + String(__DEVICE_VERSION__);
	sprite.drawString(message, IS_DEVICE_NAME_X, IS_DEVICE_NAME_Y);

	message = "Author: " + String(__DEVICE_AUTHOR__);
	sprite.drawString(message, IS_AUTHOR_NAME_X, IS_DEVICE_NAME_Y);

	_tft->pushImageDMA(IS_HEADER_X, IS_HEADER_Y, IS_HEADER_W, IS_HEADER_H, sprPtr, buffer);
	sprite.deleteSprite();


	sprPtr = (uint16_t*)sprite.createSprite(IS_NETWORK_BOX_W, IS_NETWORK_BOX_H);
	sprite.fillSprite(TFT_BLACK);

	DrawRoundedBox(&sprite, DMCoordinates(0, 0, IS_NETWORK_BOX_W, IS_NETWORK_BOX_H, 0, 0), IS_HEADER_H / 2, GlobalTheme);
	sprite.drawString("Network", 0, 0);

	_tft->pushImageDMA(IS_NETWORK_BOX_X, IS_NETWORK_BOX_Y, IS_NETWORK_BOX_W, IS_NETWORK_BOX_H, sprPtr);

	free(buffer);
	_tft->dmaWait();
	sprite.deleteSprite();
}