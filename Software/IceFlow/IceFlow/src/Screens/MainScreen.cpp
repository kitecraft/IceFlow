#include "MainScreen.h"
#include <ESPDateTime.h>
#include "Utilities/ScreenUpdateKeys.h"
#include "Utilities/DMTheme.h"
#include "Widgets/Box.h"
#include "../ProfileManager/ProfileManager.h"
#include "../DisplayManager/Utilities/CommandQueue.h"
#include "../Utilities/ControlCommands.h"

MainScreen::MainScreen(TFT_eSPI* tft)
{
	_tft = tft;

	_graphPanel = new MS_GraphPanel(_tft,
		DMCoordinates(
			0,
			0,
			MS_GRAPHPANEL_W,
			MS_GRAPHPANEL_H,
			MS_GRAPHPANEL_X,
			MS_GRAPHPANEL_Y)
	);
	//_graphPanel->IgnoreSecondary(true);
	_graphPanel->IgnoreProfile(true);

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


	CommandQueue.QueueCommand(CC_REQUEST_NET_STATUS);
	CommandQueue.QueueCommand(CC_START_TEMPERATURE_STREAM);
	//_nextGraphUpdate = millis() + UPDATE_GRAPH_RATE;
	DrawScreen();
}

MainScreen::~MainScreen()
{
	CommandQueue.QueueCommand(CC_STOP_TEMPERATURE_STREAM);
	CommandQueue.QueueCommand(CC_STOP_TIME_UPDATES);
	delete(_sideBar);
	if (_timeSprite != nullptr) {
		_timeSprite->deleteSprite();
		delete(_timeSprite);
	}
	if (_graphPanel != nullptr) {
		delete(_graphPanel);
	}
}


void MainScreen::UpdateScreen(int inKey, char* value)
{
	_tft->startWrite();
	SCREEN_UPDATE_KEYS key = static_cast<SCREEN_UPDATE_KEYS>(inKey);
	switch (key) {
	case suk_Network_Connected:
		//CommandQueue.QueueCommand(CC_START_TIME_UPATES);
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
		_tft->startWrite();
		_graphPanel->Update(_primaryTemperature, _secondaryTemperature);
		_tft->dmaWait();
		_tft->endWrite();
	}
}


void MainScreen::HandleTouch(int x, int y)
{
	if(_sideBar->Touched(x, y)){
		Serial.println("Sidebar was touched");
	}
}

void MainScreen::DrawScreen()
{
	_tft->fillScreen(TFT_BLACK);

	/*
	_tft->fillRect(0, 0, 20, 240, TFT_YELLOW);
	_tft->fillRect(20, 0, 20, 240, TFT_GREEN);
	_tft->fillRect(40, 0, 20, 240, TFT_WHITE);
	_tft->fillRect(60, 0, 20, 240, TFT_CYAN);
	_tft->fillRect(80, 0, 20, 240, 0x5AEB);
	_tft->fillRect(100, 0, 20, 240, 0x9CD3);
	_tft->fillRect(120, 0, 20, 240, 0x4208);
	_tft->fillRect(140, 0, 20, 240, 0xCE59);
	_tft->fillRect(160, 0, 20, 240, 0x738E);
	_tft->fillRect(180, 0, 20, 240, TFT_PURPLE);
	_tft->fillRect(200, 0, 20, 240, TFT_DARKGREY);
	_tft->fillRect(220, 0, 20, 240, TFT_SKYBLUE);
	_tft->fillRect(240, 0, 20, 240, TFT_BROWN);
	_tft->fillRect(260, 0, 20, 240, TFT_CASET);
	_tft->fillRect(280, 0, 20, 240, TFT_DARKGREEN);
	_tft->fillRect(300, 0, 20, 240, TFT_GREENYELLOW);
	*/

	_tft->drawSpot(WIFI_X, WIFI_Y, WIFI_SPOT_R, TFT_WHITE, TFT_TRANSPARENT);

	_tft->startWrite();
	_sideBar->Draw();
	DrawHeader();
	DrawFooter();

	_graphPanel->Draw();
	
	_tft->dmaWait();
	_tft->endWrite();

	//_graphPanel->Draw(&_currentProfile);
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