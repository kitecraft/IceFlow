#include "PreHeatScreen.h"

PreHeatScreen::PreHeatScreen(TFT_eSPI* newTFT) : ScreenBase(newTFT)
{
	_temperaturePanel.Init(newTFT);
	DrawScreen();
}

PreHeatScreen::~PreHeatScreen()
{
	//TFT->endWrite();
}

void PreHeatScreen::UpdateScreen(DISPLAY_UPDATE_KEYS inKey, String inPayload)
{
	// Keys for Display 1
	switch (inKey) {
	case DISPLAY_UPDATE_KEY_PRIMARY_TEMPERATURE:
		_temperaturePanel.UpdatePrimaryTemperature(inPayload.toDouble());
		return;
	case DISPLAY_UPDATE_KEY_SECONDARY_TEMPERATURE:
		_temperaturePanel.UpdateSecondaryTemperature(inPayload.toDouble());
		return;
	default:
		break;
	}
}

void PreHeatScreen::UpdateScreenOnInterval()
{
}

void PreHeatScreen::HandleTouch(int x, int y)
{

}

void PreHeatScreen::DrawScreen()
{
	TFT_eSprite sprite = TFT_eSprite(TFT);
	sprite.createSprite(TFT_DISPLAY_WIDTH, TFT_DISPLAY_HEIGHT);
	sprite.fillRectHGradient(0, 0, TFT_DISPLAY_WIDTH, TFT_DISPLAY_HEIGHT, TFT_WHITE, TFT_BLACK);

	_temperaturePanel.DrawPanel(&sprite);
	sprite.pushSprite(0, 0, TFT_TRANSPARENT);
	sprite.deleteSprite();
}