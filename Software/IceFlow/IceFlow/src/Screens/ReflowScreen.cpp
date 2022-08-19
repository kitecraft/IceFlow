#include "ReflowScreen.h"
#include "Utilities/ScreenNames.h"
#include "../DisplayManager/Utilities/DisplayQueue.h"

ReflowScreen::ReflowScreen(TFT_eSPI* tft)
	: BaseScreen(tft)
{
	_sidebar = new RFS_Sidebar(_tft, &_currentProfile);

	DrawScreen();
	CommandQueue.QueueCommand(CC_START_TEMPERATURE_STREAM);

	//delay(10); //give the oven controller a chance to start the reflow controller
	CommandQueue.QueueCommand(CC_REQUEST_OVEN_STATUS);
	CommandQueue.QueueCommand(CC_START_REFLOW);
}

ReflowScreen::~ReflowScreen()
{
	CommandQueue.QueueCommand(CC_STOP_TEMPERATURE_STREAM);
	CommandQueue.QueueCommand(CC_STOP_TIME_UPDATES);

	delete _sidebar;
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
		//DisplayQueue.QueueScreenChange(SN_MAIN_SCREEN);
		break;
	case suk_Oven_Reflow_On:
		_graphPanel->IgnoreTertiary(false);
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

	_sidebar->Draw();

	_tft->dmaWait();
	_tft->endWrite();
	
}