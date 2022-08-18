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

	default:
		break;
	}

	_tft->dmaWait();
	_tft->endWrite();

	/*
	switch (key) {
	case suk_PrimaryTemperature:
		UpdatePrimaryTemp(value);
		break;
	case suk_SecondaryTemperature:
		UpdateSecondaryTemp(value);
		break;
	case suk_TemperatureStreamStarted:
		_temperatureStreamStarted = true;
		_nextGraphUpdate = millis() + UPDATE_GRAPH_RATE;
		break;
	case suk_Oven_Stopped:
		DisplayQueue.QueueScreenChange(SN_MAIN_SCREEN);
		break;
	case suk_Oven_Heaters_On:
		DrawHeatersIcon(true);
		break;
	case suk_Oven_Heaters_Off:
		DrawHeatersIcon(false);
		break;
	default:
		break;
	}
	*/

}

void ReflowScreen::UpdateScreenOnInterval()
{
	_tft->startWrite();
	BaseScreen::UpdateScreenOnInterval();

	_tft->dmaWait();
	_tft->endWrite();
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