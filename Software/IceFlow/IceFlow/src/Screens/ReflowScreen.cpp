#include "ReflowScreen.h"
#include "Utilities/ScreenNames.h"
#include "../DisplayManager/Utilities/DisplayQueue.h"
#include "../Utilities/MemUseage.h"

ReflowScreen::ReflowScreen(TFT_eSPI* tft)
	: BaseScreen(tft)
{
	//Serial.println("");
	//Serial.println("ReflowScreen: Constuctor start: ");
	//PrintMemUseage();

	CommandQueue.QueueCommand(CC_START_REFLOW);
	CommandQueue.QueueCommand(CC_START_TEMPERATURE_STREAM);
	_sidebar = new RFS_Sidebar(_tft, &_currentProfile);
	_messageBox = nullptr;

	DrawScreen();

	//delay(10); //give the oven controller a chance to start the reflow controller
	CommandQueue.QueueCommand(CC_REQUEST_OVEN_STATUS);
}

ReflowScreen::~ReflowScreen()
{
	CommandQueue.QueueCommand(CC_STOP_TEMPERATURE_STREAM);
	CommandQueue.QueueCommand(CC_STOP_TIME_UPDATES);

	delete _sidebar;
	EndMessageBox();
	//Serial.println("");
	//Serial.println("ReflowScreen: Destuctor end: ");
	//PrintMemUseage();
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
		_sidebar->UpdatePreHeatTime(round(atof(value)));
		_reflowStage = RS_SOAK;
		_sidebar->EndPreHeatStage();
		_nextSideBarUpdate = 0;
		_startTime = _currentProfile.pre_heat_soak_time;
		_sidebar->UpdateSoakTime(_startTime);
		_sidebar->UpdateTarget(String(_currentProfile.pre_heat_soak_time) + " sec");
		break;
	case suk_Reflow_StageComplete_Soak:
		_startTime = millis();
		_sidebar->UpdateSoakTime(round(atof(value)));
		_sidebar->EndSoakStage();
		_sidebar->UpdateTarget(String(_currentProfile.reflow_target_temperature) + " C");
		_reflowStage = RS_RAMP;
		break;
	case suk_Reflow_StageComplete_Ramp:
		_sidebar->UpdateRampTime(round(atof(value)));
		_sidebar->EndRampStage();
		_reflowStage = RS_REFLOW;
		_nextSideBarUpdate = 0;
		_startTime = _currentProfile.reflow_soak_time;
		_sidebar->UpdateReflowTime(_startTime);
		_sidebar->UpdateTarget(String(_currentProfile.reflow_soak_time) + " sec");
		break;
	case suk_Reflow_StageComplete_Reflow:
		_startTime = millis();
		_sidebar->UpdateReflowTime(round(atof(value)));
		_sidebar->EndReflowStage();
		_reflowStage = RS_COOLING;
		_sidebar->UpdateTarget(String(_currentProfile.cooling_ramp_down_speed) + " C/sec");
		break;
	case suk_Reflow_StageComplete_Cooling:
		_sidebar->UpdateCoolingTime(round(atof(value)));
		_reflowStage = RS_COMPLETE;
		_sidebar->UpdateTarget("Fin.");
		break;
	case suk_Reflow_Complete:
		PopUpReflowCompleteMB(value);
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
		if (_activeMessageBox == RFS_NO_MB) {
			_graphPanel->Update(_primaryTemperature, _secondaryTemperature, _tertiaryTemperature);
		}
		else {
			_graphPanel->UpdateValuesOnly(_primaryTemperature, _secondaryTemperature, _tertiaryTemperature);
		}
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
	if (BaseScreen::HandleTouch(x, y)) {
		return;
	}
	ProcessTouch(x, y);
	_tft->dmaWait();
	_tft->endWrite();
}

void ReflowScreen::ProcessTouch(int x, int y)
{
	if (_activeMessageBox != RFS_NO_MB) {
		HandleMessageBoxTouch(x, y);
		return;
	}

	if (_sidebar->CancelTouched(x, y)) {
		String message = "Press continue to exit the reflow session.\nPress Cancel to return.";
		_messageBox = new MessageBox(_tft, "Cancel Reflow?", message, MESSAGE_BOX_ICON_QUESTION, MESSAGE_BOX_BUTTONS_CONTINUE_CANCEL);
		_messageBox->Show();
		_activeMessageBox = RFS_CANCEL_MB;
	}
}

void ReflowScreen::HandleMessageBoxTouch(int x, int y)
{
	DialogButtonType mbRet = _messageBox->Touched(x, y);
	if (mbRet == DB_NONE) {
		return;
	}

	switch (_activeMessageBox) {
	case RFS_CANCEL_MB:
		if (mbRet == DB_CONTINUE) {
			CommandQueue.QueueCommand(CC_STOP_OVEN);
			DisplayQueue.QueueScreenChange(SN_MAIN_SCREEN);
		}
		break;
	case RFS_COMPLETE_MB:
		CommandQueue.QueueCommand(CC_STOP_OVEN);
		DisplayQueue.QueueScreenChange(SN_MAIN_SCREEN);
		break;
	default:
		break;
	}

	EndMessageBox();
}

void ReflowScreen::PopUpReflowCompleteMB(char* runTime)
{
	int fullRunTime = round(atof(runTime));
	int minutes = fullRunTime / 60.0;
	int seconds = fullRunTime - (minutes*60);

	
	String message = "Run time: " + String(minutes) + ":";
	if (seconds < 10) {
		message += "0" + String(seconds);
	}
	else {
		message += String(seconds);
	}
	message += "\nOven and/or PCB may still be hot. Handle with care.\nThank you for using IceFlow.";
	_messageBox = new MessageBox(_tft,
		"Reflow Complete.",
		message,
		MESSAGE_BOX_ICON_INFORMATION,
		MESSAGE_BOX_BUTTONS_OK
	);
	_messageBox->Show();
	_activeMessageBox = RFS_COMPLETE_MB;
}

void ReflowScreen::EndMessageBox()
{
	if (_messageBox != nullptr) {
		_messageBox->Hide();
		delete _messageBox;
		_messageBox = nullptr;
	}
	_activeMessageBox = RFS_NO_MB;
	_graphPanel->ReDraw();
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
	_sidebar->UpdateTarget(String(_currentProfile.pre_heat_target_temperature) + " C");
}