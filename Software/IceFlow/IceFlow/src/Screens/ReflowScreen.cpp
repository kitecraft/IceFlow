#include "ReflowScreen.h"
#include "Utilities/ScreenNames.h"
#include "../DisplayManager/Utilities/DisplayQueue.h"

ReflowScreen::ReflowScreen(TFT_eSPI* tft)
	: BaseScreen(tft)
{
	CommandQueue.QueueCommand(CC_START_REFLOW);
	CommandQueue.QueueCommand(CC_START_TEMPERATURE_STREAM);
	_sidebar = new RFS_Sidebar(_tft, &_currentProfile);

	DrawScreen();

	//delay(10); //give the oven controller a chance to start the reflow controller
	CommandQueue.QueueCommand(CC_REQUEST_OVEN_STATUS);
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
	case suk_Emergency_Oven_Stopped:
		DisplayQueue.QueueScreenChange(SN_MAIN_SCREEN);
		break;
	case suk_Oven_Reflow_On:
		StartPreHeat();
		break;
	case suk_Reflow_StageComplete_PreHeat:
		_reflowStage = RS_SOAK;
		_sidebar->EndPreHeat();
		_nextSideBarUpdate = 0;
		_startTime = _currentProfile.pre_heat_soak_time;
		_sidebar->UpdateSoakTime(_startTime);
		break;
	case suk_Reflow_PreHeat_Runtime:
		_sidebar->UpdatePreHeatTime(atoi(value));
		break;
	case suk_Reflow_StageComplete_Soak:
		_reflowStage = RS_RAMP;
		_startTime = millis();
		_sidebar->EndSoak();
		break;
	case suk_Reflow_StageComplete_Ramp:
		_sidebar->EndRamp();
		_reflowStage = RS_REFLOW;
		_nextSideBarUpdate = 0;
		_startTime = _currentProfile.reflow_soak_time;
		_sidebar->UpdateReflowTime(_startTime);
		break;
	case suk_Reflow_Ramp_Runtime:
		_sidebar->UpdateRampTime(atoi(value));
		break;
	case suk_Reflow_StageComplete_Reflow:
		_reflowStage = RS_COOLING;
		_startTime = millis();
		_sidebar->EndReflow();
		break;
	case suk_Reflow_Complete:
		_reflowStage = RS_COMPLETE;
		break;
	default:
		break;
	}

	_tft->dmaWait();
	_tft->endWrite();
}

void ReflowScreen::UpdateScreenOnInterval()
{
	_tft->startWrite();
	if (_temperatureStreamStarted && (_nextGraphUpdate < millis())) {
		_nextGraphUpdate = millis() + UPDATE_GRAPH_RATE;
		_graphPanel->Update(_primaryTemperature, _secondaryTemperature, _tertiaryTemperature);
	}

	switch (_reflowStage) {
	case RS_PREHEAT:
		if (_nextSideBarUpdate < millis()) {
			_nextSideBarUpdate = millis() + 1000;
			_sidebar->UpdatePreHeatTime((float)(millis() - _startTime) / 1000.0);
		}
		break;
	case RS_SOAK:
		if (_nextSideBarUpdate < millis()) {
			_nextSideBarUpdate = millis() + 1000;
			_startTime--;
			_sidebar->UpdateSoakTime(_startTime);
		}
		break;
	case RS_RAMP:
		if (_nextSideBarUpdate < millis()) {
			_nextSideBarUpdate = millis() + 1000;
			_sidebar->UpdateRampTime((float)(millis() - _startTime) / 1000.0);
		}
		break;
	case RS_REFLOW:
		if (_nextSideBarUpdate < millis()) {
			_nextSideBarUpdate = millis() + 1000;
			_startTime--;
			_sidebar->UpdateReflowTime(_startTime);
		}
		break;
	case RS_COOLING:
		if (_nextSideBarUpdate < millis()) {
			_nextSideBarUpdate = millis() + 1000;
			_sidebar->UpdateCoolingTime((float)(millis() - _startTime) / 1000.0);
		}
		break;

	default:
		break;
	}

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

	_sidebar->Draw();

	_tft->dmaWait();
	_tft->endWrite();
	
}

void ReflowScreen::StartPreHeat()
{
	_graphPanel->IgnoreTertiary(false);
	_nextSideBarUpdate = 0;
	_startTime = millis();
	_reflowStage = RS_PREHEAT;
}