#include "ReflowScreen.h"

ReflowScreen::ReflowScreen(TFT_eSPI* tft)
	: BaseScreen(tft)
{
	DrawScreen();
	CommandQueue.QueueCommand(CC_START_TEMPERATURE_STREAM);
	CommandQueue.QueueCommand(CC_REQUEST_OVEN_STATUS);
}

ReflowScreen::~ReflowScreen()
{
	CommandQueue.QueueCommand(CC_STOP_TEMPERATURE_STREAM);
	CommandQueue.QueueCommand(CC_STOP_TIME_UPDATES);
}

void ReflowScreen::UpdateScreen(int inKey, char* value)
{
	SCREEN_UPDATE_KEYS key = static_cast<SCREEN_UPDATE_KEYS>(inKey);

	_tft->startWrite();
	if (BaseScreen::UpdateScreen(key, value)) {
		_tft->dmaWait();
		_tft->endWrite();
		return;
	}

	switch (key) {
	case suk_Oven_Stopped:
		break;

	default:
		break;
	}

	_tft->dmaWait();
	_tft->endWrite();
}

void ReflowScreen::UpdateScreenOnInterval()
{
	if (_temperatureStreamStarted && (_nextGraphUpdate < millis())) {
		_nextGraphUpdate = millis() + UPDATE_GRAPH_RATE;
		_tft->startWrite();
		_graphPanel->Update(_primaryTemperature, _secondaryTemperature, _tertiaryTemperature);

		_tft->dmaWait();
		_tft->endWrite();
	}
}

void ReflowScreen::HandleTouch(int x, int y)
{
	_tft->startWrite();
	ProcessTouch(x, y);
	_tft->dmaWait();
	_tft->endWrite();
}

void ReflowScreen::ProcessTouch(int x, int y)
{

}

void ReflowScreen::DrawScreen()
{
	_tft->startWrite();
	BaseScreen::DrawScreen();

	TFT_eSprite sprite(_tft);
	uint16_t* sprPtr = (uint16_t*)sprite.createSprite(BASE_SIDEBAR_W, 240);
	sprite.fillSprite(TFT_BLACK);

	sprite.setFreeFont(SMALL_FONT);
	sprite.setTextColor(GlobalTheme.textColor, TFT_BLACK);
	sprite.setTextDatum(TR_DATUM);
	sprite.drawString("Pre-Heat", BASE_SIDEBAR_W, 2);

	sprite.setTextColor(GlobalTheme.panelBorderColor, TFT_BLACK);
	sprite.drawString("Soak", BASE_SIDEBAR_W, 42);

	sprite.drawString("Ramp", BASE_SIDEBAR_W, 82);

	sprite.drawString("ReFlow", BASE_SIDEBAR_W, 122);

	sprite.drawString("Cooling", BASE_SIDEBAR_W, 162);
	_tft->pushImageDMA(BASE_SIDEBAR_X, 0, BASE_SIDEBAR_W, 240, sprPtr);


	_tft->dmaWait();
	_tft->endWrite();

	sprite.deleteSprite();

}