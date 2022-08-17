#include "MainScreen.h"
#include <ESPDateTime.h>
#include "Utilities/ScreenUpdateKeys.h"
#include "Utilities/DMTheme.h"
#include "Widgets/Box.h"
#include "../ProfileManager/ProfileManager.h"
#include "../DisplayManager/Utilities/CommandQueue.h"
#include "../Utilities/ControlCommands.h"
#include "../Utilities/MemUseage.h"
#include "../Utilities/PreferencesManager.h"

MainScreen::MainScreen(TFT_eSPI* tft)
{
	Serial.println("");
	Serial.println("MainScreen: Constuctor start: ");
	PrintMemUseage();
	_tft = tft;
	_targetTemperatureDlg = nullptr;
	_messageBox = nullptr;


	_graphPanel = new MS_GraphPanel(_tft,
		DMCoordinates(
			0,
			0,
			MS_GRAPHPANEL_W,
			MS_GRAPHPANEL_H,
			MS_GRAPHPANEL_X,
			MS_GRAPHPANEL_Y),
		false,
		true
	);

	_timeSprite = new TFT_eSprite(_tft);
	_timeSprite->createSprite(TIME_W, TIME_H);
	_timeSprite->setFreeFont(SMALL_FONT);
	_timeSprite->setTextColor(GlobalTheme.textColor, TFT_BLACK);
	_timeSprite->setTextDatum(TR_DATUM);

	if (!ProfileManager.GetSavedProfile(&_currentProfile)) {
		Serial.println("Failed to load Profile");
	}
	_sideBar = new SideBar(_tft,DMCoordinates(SIDEBAR_X, SIDEBAR_Y, SIDEBAR_W, SIDEBAR_H, SIDEBAR_X, SIDEBAR_Y));


	DMTheme temperatureTheme = GlobalTheme;
	temperatureTheme.textColor = PRIMARY_TEMPERATURE_COLOR;

	_primaryTemperatureTB = new TextBox(TextBoxDto(
		DMCoordinates(
			MS_PRIMARY_TEMPERATURE_TB_X,
			MS_HEADER_TB_Y,
			MS_TEMPERATURE_TB_W,
			MEDIUM_FONT_TEXT_BOX_H,
			MS_FOOTER_X + MS_PRIMARY_TEMPERATURE_TB_X,
			MS_FOOTER_Y + MS_HEADER_TB_Y
		),
		temperatureTheme,
		MEDIUM_FONT,
		MR_DATUM,
		false,
		true,
		GlobalTheme.panelLightColor),
		_tft);

	temperatureTheme.textColor = SEC_TEMPERATURE_COLOR;
	_secondaryTemperatureTB = new TextBox(TextBoxDto(
		DMCoordinates(
			MS_SEC_TEMPERATURE_TB_X,
			MS_HEADER_TB_Y,
			MS_TEMPERATURE_TB_W,
			MEDIUM_FONT_TEXT_BOX_H,
			MS_FOOTER_X + MS_SEC_TEMPERATURE_TB_X,
			MS_FOOTER_Y + MS_HEADER_TB_Y
		),
		temperatureTheme,
		MEDIUM_FONT,
		MR_DATUM,
		false,
		true,
		GlobalTheme.panelLightColor),
		_tft);


	DrawScreen();
	CommandQueue.QueueCommand(CC_REQUEST_NET_STATUS);
	CommandQueue.QueueCommand(CC_START_TEMPERATURE_STREAM);
	CommandQueue.QueueCommand(CC_REQUEST_OVEN_STATUS);
	_nextGraphUpdate = millis() + UPDATE_GRAPH_RATE;

	//Serial.println("Constuctor end: ");
	//PrintMemUseage();
	Serial.println("");
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
	delete(_graphPanel);

	delete(_primaryTemperatureTB);
	delete(_secondaryTemperatureTB);

	if (_targetTemperatureDlg != nullptr) {
		delete(_targetTemperatureDlg);
	}

	if (_messageBox != nullptr) {
		delete _messageBox;
	}

	Serial.println("");
	Serial.println("MainScreen: Destuctor end: ");
	PrintMemUseage();
}


void MainScreen::UpdateScreen(int inKey, char* value)
{
	_tft->startWrite();
	SCREEN_UPDATE_KEYS key = static_cast<SCREEN_UPDATE_KEYS>(inKey);
	switch (key) {
	case suk_Network_Connected:
		CommandQueue.QueueCommand(CC_START_TIME_UPATES);
		_tft->drawSpot(WIFI_X, WIFI_Y, WIFI_SPOT_R, TFT_GREEN, TFT_TRANSPARENT);
		break;
	case suk_Network_Started:
		_tft->drawSpot(WIFI_X, WIFI_Y, WIFI_SPOT_R, TFT_YELLOW, TFT_TRANSPARENT);
		break;
	case suk_DateTime:
		DisplayTime();
		break;
	case suk_PrimaryTemperature:
		UpdatePrimaryTemp(value);
		break;
	case suk_SecondaryTemperature:
		UpdateSecondaryTemp(value);
		break;
	case suk_TemperatureStreamStarted:
		_temperatureStreamStarted = true;
		_nextGraphUpdate = millis() + UPDATE_GRAPH_RATE;
	case suk_Oven_Manual_On:
		_tertiaryTermperature = atof(value);
		_graphPanel->IgnoreTertiary(false);
		break;
	case suk_Oven_Stopped:
		_graphPanel->IgnoreTertiary(true);
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
	_tft->dmaWait();
	_tft->endWrite();
}

void MainScreen::UpdateScreenOnInterval()
{
	if (_temperatureStreamStarted && (_nextGraphUpdate < millis())) {
		_nextGraphUpdate = millis() + UPDATE_GRAPH_RATE;

		if (_sideBar->IsPopUpOpen() || _targetTemperatureDlg != nullptr) {
			_graphPanel->UpdateValuesOnly(_primaryTemperature, _secondaryTemperature, _tertiaryTermperature);
		}
		else {
			_tft->startWrite();
			_graphPanel->Update(_primaryTemperature, _secondaryTemperature, _tertiaryTermperature);

			_tft->dmaWait();
			_tft->endWrite();
		}
	}
}


void MainScreen::HandleTouch(int x, int y)
{
	_tft->startWrite();
	ProcessTouch(x, y);
	_tft->dmaWait();
	_tft->endWrite();
}

void MainScreen::ProcessTouch(int x, int y)
{

	// Handle message box if active
	if (_activeMessageBox != MS_NO_MB) {
		HandleMessageBoxTouch(x, y);
		return;
	}

	if (_targetTemperatureDlg != nullptr) {
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
		break;
	case SB_START_MANUAL_HEAT:
		ManualHeatTouched();
		return;
	case SB_START_AUTO_TUNE:

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
	default:
		break;
	}

}

void MainScreen::EndMessageBox()
{
	_messageBox->Hide();
	delete _messageBox;
	_messageBox = nullptr;
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
	_tft->drawSpot(WIFI_X, WIFI_Y, WIFI_SPOT_R, TFT_WHITE, TFT_TRANSPARENT);

	_tft->startWrite();
	_sideBar->Draw();
	DrawHeader();
	DrawFooter();
	DrawHeatersIcon(false);
	_graphPanel->Draw();
	
	_tft->dmaWait();
	_tft->endWrite();
}

void MainScreen::DrawHeader()
{
	TFT_eSprite sprite(_tft);
	uint16_t* sprPtr = (uint16_t*)sprite.createSprite(MS_HEADER_W, MS_HEADER_H);

	sprite.fillSprite(TFT_BLACK);
	DrawRoundedBox(&sprite, DMCoordinates(0, 0, MS_HEADER_W, MS_HEADER_H, 0, 0), MS_HEADER_H / 2, GlobalTheme);

	sprite.setFreeFont(LARGE_FONT);
	sprite.setTextColor(GlobalTheme.textColor, GlobalTheme.panelLightColor);
	sprite.drawString(__DEVICE_NAME__, MS_DEVICE_NAME_X, MS_DEVICE_NAME_Y);

	int profileLableX = MS_DEVICE_NAME_X + sprite.textWidth(__DEVICE_NAME__) + MS_PROFILE_LABEL_X_OFFSET;

	sprite.setFreeFont(MEDIUM_FONT);
	sprite.setTextColor(TFT_GREEN, GlobalTheme.panelLightColor);
	sprite.setTextDatum(ML_DATUM);
	sprite.drawString(MS_PROFILE_LABEL, profileLableX, (MS_HEADER_H /2) - 1);

	int profileX = profileLableX + sprite.textWidth(MS_PROFILE_LABEL);
	TextBox::DrawTextBox(&sprite,
		TextBoxDto(
			DMCoordinates(
				profileX,
				MS_HEADER_TB_Y,
				-1,
				MEDIUM_FONT_TEXT_BOX_H,
				MS_DEVICE_NAME_X + MS_HEADER_X,
				MS_HEADER_TB_Y + MS_HEADER_Y),
			GlobalTheme,
			MEDIUM_FONT,
			MC_DATUM,
			false,
			true,
			GlobalTheme.panelLightColor),
		_currentProfile.name.c_str());

	_tft->pushImageDMA(MS_HEADER_X, MS_HEADER_Y, MS_HEADER_W, MS_HEADER_H, sprPtr);
	_tft->dmaWait();
	sprite.deleteSprite();
}
void MainScreen::DrawFooter()
{
	TFT_eSprite sprite(_tft);
	uint16_t* sprPtr = (uint16_t*)sprite.createSprite(MS_FOOTER_W, MS_FOOTER_H);

	sprite.fillSprite(TFT_BLACK);
	DrawRoundedBox(&sprite, DMCoordinates(0, 0, MS_FOOTER_W, MS_FOOTER_H, 0, 0), MS_FOOTER_H / 2, GlobalTheme);

	sprite.setFreeFont(MEDIUM_FONT);
	sprite.setTextDatum(ML_DATUM);

	sprite.setTextColor(PRIMARY_TEMPERATURE_COLOR, GlobalTheme.panelLightColor);
	sprite.drawString("Prim: ", MS_PRIMARY_TEMPERATURE_LABEL_X, MS_TEMPERATURE_LABEL_Y);

	sprite.setTextColor(SEC_TEMPERATURE_COLOR, GlobalTheme.panelLightColor);
	sprite.drawString("Sec: ", MS_SEC_TEMPERATURE_LABEL_X, MS_TEMPERATURE_LABEL_Y);
	
	_primaryTemperatureTB->Draw(&sprite, "888.88 C");
	_secondaryTemperatureTB->Draw(&sprite, "888.88 C");

	_tft->pushImageDMA(MS_FOOTER_X, MS_FOOTER_Y, MS_FOOTER_W, MS_FOOTER_H, sprPtr);
	_tft->dmaWait();
	sprite.deleteSprite();
}

void MainScreen::DisplayTime()
{
	_timeSprite->fillSprite(TFT_BLACK);
	_timeSprite->drawString(DateTime.format(DateFormatter::TIME_ONLY), TIME_W, 2);
	_timeSprite->pushSprite(TIME_X, TIME_Y);
}

void MainScreen::UpdatePrimaryTemp(char* val)
{
	char value[9];
	snprintf(value, 9, "%s C", val);
	_primaryTemperatureTB->Update(value);
	_primaryTemperature = atof(val);
}

void MainScreen::UpdateSecondaryTemp(char* val)
{
	char value[9];
	snprintf(value, 9, "%s C", val);
	_secondaryTemperatureTB->Update(value);
	_secondaryTemperature = atof(val);
}

void MainScreen::ManualHeatTouched()
{
	if (_targetTemperatureDlg != nullptr) {
		delete(_targetTemperatureDlg);
	}

	_targetTemperatureDlg = new TargetTemperatureDlg(_tft, MANAUAL_HEAT_TARGET_TEMPERATURE_DIALOG_TITLE);
	_targetTemperatureDlg->SetTargetTemperature(GetMaualHeatTargetTemperature());
	_targetTemperatureDlg->Show();
}

bool MainScreen::ManualHeatDlgClosed(DialogButtonType action)
{
	int targetTemp = _targetTemperatureDlg->GetTargetTemperature();

	if (action == DB_Continue) {
		int highest = GetOvenDoNotExceedTemperature();
		if (targetTemp > highest - 10) {
			String message = "OVEN SAFETY LIMIT\nTarget can not be higher than '" + String(highest - 10) + " degrees Celcius'";
			_messageBox = new MessageBox(_tft, "Over Limit", message, MESSAGE_BOX_ERROR, MESSAGE_BOX_OK);
			_messageBox->Show();
			_activeMessageBox = MS_OVER_MAX_TEMP;
			return false;
		}

		char val[4];
		snprintf(val, 7, "%i", targetTemp);
		CommandQueue.QueueCommandAndValue(CC_START_MANUAL_HEAT, val);
		SaveMaualHeatTargetTemperature(targetTemp);
	}

	_targetTemperatureDlg->Hide();
	delete _targetTemperatureDlg;
	_targetTemperatureDlg = nullptr;

	return true;
}

void MainScreen::DrawHeatersIcon(bool status)
{
	int color = GlobalTheme.panelDarkColor;
	if (status) {
		color = TFT_RED;
	}

	TFT_eSprite sprite(_tft);
	uint16_t* sprPtr = (uint16_t*)sprite.createSprite(MS_HEATER_ICON_W, MS_HEATER_ICON_H);

	sprite.fillSprite(GlobalTheme.panelLightColor);
	sprite.drawRoundRect(0, 3, MS_HEATER_ICON_W, MS_HEATER_ICON_H - 6, 5, GlobalTheme.panelBorderColor);
	sprite.drawRoundRect(1, 4, MS_HEATER_ICON_W-2, MS_HEATER_ICON_H - 8, 5, GlobalTheme.panelBorderColor);
	sprite.fillSmoothRoundRect(2, 5, MS_HEATER_ICON_W-4, MS_HEATER_ICON_H - 10, 5, color, GlobalTheme.panelBorderColor);
	
	_tft->pushImageDMA(MS_HEATER_ICON_X, MS_HEATER_ICON_Y, MS_HEATER_ICON_W, MS_HEATER_ICON_H, sprPtr);
	_tft->dmaWait();
	sprite.deleteSprite();
}