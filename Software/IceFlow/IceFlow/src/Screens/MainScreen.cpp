#include "MainScreen.h"
#include <ESPDateTime.h>
#include "Utilities/DMTheme.h"
#include "Widgets/Box.h"
#include "../Utilities/PreferencesManager.h"
#include "Utilities/ScreenNames.h"
#include "../DisplayManager/Utilities/DisplayQueue.h"
#include "../Utilities/MemUseage.h"

MainScreen::MainScreen(TFT_eSPI* tft) : BaseScreen(tft)
{
	//Serial.println("");
	//Serial.println("MainScreen: Constuctor start: ");
	//PrintMemUseage();
	_targetTemperatureDlg = nullptr;
	_activeTargetTempDLG = MSTDLG_NONE;
	_messageBox = nullptr;
	_activeMessageBox = MS_NO_MB;
	_pidEditor = nullptr;
	_pidEditorActive = false;

	_timeSprite = new TFT_eSprite(_tft);
	_timeSprite->createSprite(BASE_TIME_W, BASE_TIME_H);
	_timeSprite->setFreeFont(SMALL_FONT);
	_timeSprite->setTextColor(GlobalTheme.textColor, TFT_BLACK);
	_timeSprite->setTextDatum(TR_DATUM);


	_sideBar = new SideBar(_tft,DMCoordinates(BASE_SIDEBAR_X, BASE_SIDEBAR_Y, SIDEBAR_W, SIDEBAR_H, BASE_SIDEBAR_X, BASE_SIDEBAR_Y));
	ResetSideBarIcons();
	DrawScreen();

	CommandQueue.QueueCommand(CC_REQUEST_NET_STATUS);
	CommandQueue.QueueCommand(CC_START_TEMPERATURE_STREAM);
	CommandQueue.QueueCommand(CC_REQUEST_OVEN_STATUS);

	//Serial.println("Constuctor end: ");
	//PrintMemUseage();
	//Serial.println("");
}

MainScreen::~MainScreen()
{
	//Serial.println("");
	//Serial.println("MainScreen: Destuctor start: ");
	//PrintMemUseage();

	CommandQueue.QueueCommand(CC_STOP_TEMPERATURE_STREAM);
	CommandQueue.QueueCommand(CC_STOP_TIME_UPDATES);
	

	_timeSprite->deleteSprite();
	delete(_timeSprite);
	delete(_sideBar);
	EndTargetTempDlg();

	if (_messageBox != nullptr) {
		delete _messageBox;
	}

	if (_pidEditorActive) {
		delete _pidEditor;
	}

	//Serial.println("");
	//Serial.println("MainScreen: Destuctor end: ");
	//PrintMemUseage();
}


void MainScreen::UpdateScreen(int inKey, char* value)
{

	SCREEN_UPDATE_KEYS key = static_cast<SCREEN_UPDATE_KEYS>(inKey);

	_tft->startWrite();
	if (BaseScreen::UpdateScreen(key, value)) {
		_tft->dmaWait();
		_tft->endWrite();
		return;
	}

	switch (key) {
	case suk_Network_Connected:
		CommandQueue.QueueCommand(CC_START_TIME_UPATES);
		_tft->drawSpot(BASE_WIFI_X, BASE_WIFI_Y, BASE_WIFI_SPOT_R, TFT_GREEN, TFT_TRANSPARENT);
		break;
	case suk_Network_Started:
		_tft->drawSpot(BASE_WIFI_X, BASE_WIFI_Y, BASE_WIFI_SPOT_R, TFT_YELLOW, TFT_TRANSPARENT);
		break;
	case suk_DateTime:
		DisplayTime();
		break;
	case suk_Oven_Manual_On:
		_tertiaryTemperature = atof(value);
		_graphPanel->IgnoreTertiary(false);
		_sideBar->StopIconEnabled(true);
		_sideBar->Draw();
		break;
	case suk_Oven_Stopped:
	case suk_Emergency_Oven_Stopped:
		_graphPanel->IgnoreTertiary(true);
		ResetSideBarIcons();
		_sideBar->Draw();
		break;
	case suk_Oven_Heaters_On:
		DrawHeatersIcon(true);
		break;
	case suk_Oven_Heaters_Off:
		DrawHeatersIcon(false);
		break;
	case suk_Oven_AutoTune_On:
		DisableAllSideBarIconsEnableStop();
		_sideBar->Draw();
		_tertiaryTemperature = atof(value);
		_graphPanel->IgnoreTertiary(false);
		break;
	case suk_Oven_AutoTune_Complete:
		_graphPanel->IgnoreTertiary(true);
		ResetSideBarIcons();
		_sideBar->Draw();
		//display results here
		_pidEditor = new PidEditor(_tft);
		_pidEditor->Show();
		_pidEditorActive = true;
		break;
	case suk_Oven_Exceeded_Max:
		_messageBox = new MessageBox(_tft, "Oven temperature!", "The oven exceeded maximum temperature of '" + String(GetOvenDoNotExceedTemperature()) + " degrees Celcius' and was killed.", MESSAGE_BOX_ICON_ERROR, MESSAGE_BOX_BUTTONS_OK);
		_messageBox->Show();
		_activeMessageBox = MS_OVEN_EXCEEDED_MAX;
		break;
	default:
		break;
	}

	_tft->dmaWait();
	_tft->endWrite();
}

void MainScreen::UpdateScreenOnInterval()
{
	if (_temperatureStreamStarted && (_nextGraphUpdate < millis())) {
		_nextGraphUpdate = millis() + UPDATE_GRAPH_RATE;

		if (_sideBar->IsPopUpOpen() ||
			_activeTargetTempDLG != MSTDLG_NONE ||
			_activeMessageBox != MS_NO_MB||
			_activeMessageBox != MS_NO_MB||
			_pidEditorActive == true ) {
			_graphPanel->UpdateValuesOnly(_primaryTemperature, _secondaryTemperature, _tertiaryTemperature);
		}
		else {
			_tft->startWrite();
			_graphPanel->Update(_primaryTemperature, _secondaryTemperature, _tertiaryTemperature);

			_tft->dmaWait();
			_tft->endWrite();
		}
	}
}


void MainScreen::HandleTouch(int x, int y)
{
	_tft->startWrite();
	if (BaseScreen::HandleTouch(x, y)) {
		return;
	}
	ProcessTouch(x, y);
	_tft->dmaWait();
	_tft->endWrite();
}

void MainScreen::ProcessTouch(int x, int y)
{
	if (_pidEditorActive == true) {
		DialogButtonType ret = _pidEditor->Touched(x, y);

		if (ret == DB_CLOSE) {
			_pidEditor->Hide();
			CommandQueue.QueueCommand(CC_OVEN_RESET_PIDS);
			delete _pidEditor;
			_pidEditor = nullptr;
			_pidEditorActive = false;
			_graphPanel->ReDraw();
		}
		else if (ret == DB_CONTINUE) {
			_pidEditor->Hide();
			delete _pidEditor;
			_pidEditor = nullptr;
			_pidEditorActive = false;
			_graphPanel->ReDraw();
			StartAutoTune();
		}
		return;
	}

	// Handle message box if active
	if (_activeMessageBox != MS_NO_MB) {
		HandleMessageBoxTouch(x, y);
		return;
	}

	if(_activeTargetTempDLG != MSTDLG_NONE){
		DialogButtonType result = _targetTemperatureDlg->Touched(x, y);
		if (result != DB_NONE) {
			ManualHeatDlgClosed(result);
		}
		return;
	}


	SB_TOUCHED_RETURN ret = _sideBar->Touched(x, y);
	switch (ret) {
	case SB_MENU_CLOSED:
		_graphPanel->ReDraw();
		return;
	case SB_START_MANUAL_HEAT:
		ManualHeatTouched();
		return;
	case SB_AUTOTUNE_TOUCHED:
		//StartAutoTune();
		_pidEditor = new PidEditor(_tft);
		_pidEditor->Show();
		_pidEditorActive = true;
		return;
	case SB_INFO_TOUCHED:
		DisplayQueue.QueueScreenChange(SN_INFO_SCREEN);
		break;
	case SB_START_REFLOW:
		ReflowTouched();
		break;
	case SB_STOP:
		CommandQueue.QueueCommand(CC_STOP_OVEN);
		break;
	default:
		break;
	}
}

void MainScreen::HandleMessageBoxTouch(int x, int y)
{
	DialogButtonType mbRet = _messageBox->Touched(x, y);
	if (mbRet == DB_NONE) {
		return;
	}

	switch (_activeMessageBox) {
	case MS_OVER_MAX_TEMP:
		EndMessageBox();
		_targetTemperatureDlg->SetTargetTemperature(GetMaualHeatTargetTemperature());
	case MS_START_REFLOW_TEMP_ERROR_MB:
		EndMessageBox();
		break;
	case MS_START_REFLOW_MB:
		if (mbRet == DB_CONTINUE) {
			DisplayQueue.QueueScreenChange(SN_REFLOW_SCREEN);
		}
		EndMessageBox();
		break;
	case MS_OVEN_EXCEEDED_MAX:
		EndMessageBox();
		break;
	default:
		break;
	}
}

void MainScreen::EndMessageBox()
{
	if (_messageBox != nullptr) {
		_messageBox->Hide();
		delete _messageBox;
		_messageBox = nullptr;
	}

	if(_activeTargetTempDLG == MSTDLG_NONE){
		_graphPanel->ReDraw();
	}

	_activeMessageBox = MS_NO_MB;
}

bool MainScreen::HandleOKMessageBox(DialogButtonType buttonPressed)
{
	if (buttonPressed != DB_OK) {
		return false;
	}

	EndMessageBox();
	return true;
}

void MainScreen::DrawScreen()
{
	_tft->fillScreen(TFT_BLACK);
	_tft->drawSpot(BASE_WIFI_X, BASE_WIFI_Y, BASE_WIFI_SPOT_R, TFT_WHITE, TFT_TRANSPARENT);

	_tft->startWrite();
	BaseScreen::DrawScreen();

	_sideBar->Draw();

	_tft->dmaWait();
	_tft->endWrite();
}

void MainScreen::DisplayTime()
{
	_timeSprite->fillSprite(TFT_BLACK);
	_timeSprite->drawString(DateTime.format(DateFormatter::TIME_ONLY), BASE_TIME_W, 2);
	_timeSprite->pushSprite(BASE_TIME_X, BASE_TIME_Y);
}

void MainScreen::ReflowTouched()
{
	if (_primaryTemperature > MINIUM_OVEN_TEMPERATURE_REFLOW_START) {
		String message = "Oven temperature must be below '" + String(MINIUM_OVEN_TEMPERATURE_REFLOW_START) + " degrees Celcius' to begin a Reflow session";
		_messageBox = new MessageBox(_tft, "Over Minimum", message, MESSAGE_BOX_ICON_ERROR, MESSAGE_BOX_BUTTONS_OK);
		_messageBox->Show();
		_activeMessageBox = MS_START_REFLOW_TEMP_ERROR_MB;
		return;
	}
	
	String message = "Start a Reflow sesion with '" + _currentProfile.name + "' ?";
	_messageBox = new MessageBox(_tft, "Start Reflow", message, MESSAGE_BOX_ICON_QUESTION, MESSAGE_BOX_BUTTONS_CONTINUE_CANCEL);
	_messageBox->Show();
	_activeMessageBox = MS_START_REFLOW_MB;
}

void MainScreen::StartAutoTune()
{
	_targetTemperatureDlg = new TargetTemperatureDlg(_tft, AUTOTUNE_TARGET_TEMPERATURE_DIALOG_TITLE);
	_targetTemperatureDlg->SetTargetTemperature(GetMaualHeatTargetTemperature());
	_targetTemperatureDlg->Show();
	_activeTargetTempDLG = MSTDLG_AUTO_TUNE;
}

void MainScreen::ManualHeatTouched()
{
	EndTargetTempDlg();

	_targetTemperatureDlg = new TargetTemperatureDlg(_tft, MANAUAL_HEAT_TARGET_TEMPERATURE_DIALOG_TITLE);
	_targetTemperatureDlg->SetTargetTemperature(GetMaualHeatTargetTemperature());
	_targetTemperatureDlg->Show();
	_activeTargetTempDLG = MSTDLG_MANUAL_HEAT;
}

bool MainScreen::ManualHeatDlgClosed(DialogButtonType action)
{
	int targetTemp = _targetTemperatureDlg->GetTargetTemperature();

	if (action == DB_CONTINUE) {
		int highest = GetOvenDoNotExceedTemperature();
		if (targetTemp > highest - 10) {
			String message = "OVEN SAFETY LIMIT\nTarget can not be higher than '" + String(highest - 10) + " degrees Celcius'";
			_messageBox = new MessageBox(_tft, "Over Limit.", message, MESSAGE_BOX_ICON_ERROR, MESSAGE_BOX_BUTTONS_OK);
			_messageBox->Show();
			_activeMessageBox = MS_OVER_MAX_TEMP;
			return false;
		}

		char val[4];
		snprintf(val, 7, "%i", targetTemp);
		if (_activeTargetTempDLG == MSTDLG_MANUAL_HEAT) {
			CommandQueue.QueueCommandAndValue(CC_START_MANUAL_HEAT, val);
			SaveMaualHeatTargetTemperature(targetTemp);
		}
		else if (_activeTargetTempDLG == MSTDLG_AUTO_TUNE) {
			CommandQueue.QueueCommandAndValue(CC_START_AUTOTUNE, val);
		}
		
	}

	EndTargetTempDlg();
	_graphPanel->ReDraw();
	return true;
}

void MainScreen::EndTargetTempDlg()
{
	if (_targetTemperatureDlg != nullptr) {
		_targetTemperatureDlg->Hide();
		delete _targetTemperatureDlg;
		_targetTemperatureDlg = nullptr;

	}
	_activeTargetTempDLG = MSTDLG_NONE;
}

void MainScreen::DrawHeatersIcon(bool status)
{
	int color = GlobalTheme.panelDarkColor;
	if (status) {
		color = TFT_RED;
	}

	TFT_eSprite sprite(_tft);
	uint16_t* sprPtr = (uint16_t*)sprite.createSprite(BASE_HEATER_ICON_W, BASE_HEATER_ICON_H);

	sprite.fillSprite(GlobalTheme.panelLightColor);
	sprite.drawRoundRect(0, 3, BASE_HEATER_ICON_W, BASE_HEATER_ICON_H - 6, 5, GlobalTheme.panelBorderColor);
	sprite.drawRoundRect(1, 4, BASE_HEATER_ICON_W-2, BASE_HEATER_ICON_H - 8, 5, GlobalTheme.panelBorderColor);
	sprite.fillSmoothRoundRect(2, 5, BASE_HEATER_ICON_W-4, BASE_HEATER_ICON_H - 10, 5, color, GlobalTheme.panelBorderColor);
	
	_tft->pushImageDMA(BASE_HEATER_ICON_X, BASE_HEATER_ICON_Y, BASE_HEATER_ICON_W, BASE_HEATER_ICON_H, sprPtr);
	_tft->dmaWait();
	sprite.deleteSprite();
}

void MainScreen::ResetSideBarIcons()
{
	_sideBar->SettingsIconEnabled(true);
	_sideBar->ProfileIconEnabled(true);
	_sideBar->ManualHeatIconEnabled(true);
	_sideBar->ReflowIconEnabled(true);
	_sideBar->StopIconEnabled(false);
}

void MainScreen::DisableAllSideBarIconsEnableStop()
{
	_sideBar->SettingsIconEnabled(false);
	_sideBar->ProfileIconEnabled(false);
	_sideBar->ManualHeatIconEnabled(false);
	_sideBar->ReflowIconEnabled(false);
	_sideBar->StopIconEnabled(true);
}