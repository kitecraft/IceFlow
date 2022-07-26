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
	_wifiX = _tft->width() - 4;
	_timeX = _wifiX - TIME_W - 7;
	_sidebarX = _tft->width() - SIDEBAR_W;
	_headerW = _sidebarX - HEADER_W_OFFSET;
	_timeSprite = new TFT_eSprite(_tft);
	_timeSprite->createSprite(TIME_W, TIME_H);
	_timeSprite->setFreeFont(SMALL_FONT);
	_timeSprite->setTextColor(GlobalTheme.textColor, TFT_BLACK);
	_timeSprite->setTextDatum(TR_DATUM);

	if (!ProfileManager.GetSavedProfile(&_currentProfile)) {
		Serial.println("Failed to load Profile");
	}
	_sideBar = new SideBar(_tft,DMCoordinates(_sidebarX, SIDEBAR_Y, SIDEBAR_W, SIDEBAR_H, _sidebarX, SIDEBAR_Y));
	CommandQueue.QueueCommand(CC_REQUEST_NET_STATUS);
	DrawScreen();
}

MainScreen::~MainScreen()
{
	delete(_sideBar);
	if (_timeSprite != nullptr) {
		_timeSprite->deleteSprite();
		delete(_timeSprite);
	}
}


void MainScreen::UpdateScreen(int inKey, char* value)
{
	_tft->startWrite();
	SCREEN_UPDATE_KEYS key = static_cast<SCREEN_UPDATE_KEYS>(inKey);
	switch (key) {
	case suk_Network_Connected:
		_tft->drawSpot(_wifiX, WIFI_Y, WIFI_SPOT_R, TFT_GREEN, TFT_TRANSPARENT);
		break;
	case suk_Network_Started:
		_tft->drawSpot(_wifiX, WIFI_Y, WIFI_SPOT_R, TFT_YELLOW, TFT_TRANSPARENT);
		break;
	case suk_DateTime:
		DisplayTime();
		break;
	default:
		break;
	}
	_tft->dmaWait();
	_tft->endWrite();
}

void MainScreen::UpdateScreenOnInterval()
{		
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

	_tft->drawSpot(_wifiX, WIFI_Y, WIFI_SPOT_R, TFT_WHITE, TFT_TRANSPARENT);

	_tft->startWrite();
	_sideBar->Draw();
	DrawHeader();

	_tft->dmaWait();
	_tft->endWrite();
}

void MainScreen::DrawHeader()
{
	TFT_eSprite sprite(_tft);
	uint16_t* sprPtr = (uint16_t*)sprite.createSprite(_headerW, HEADER_H);

	sprite.fillSprite(TFT_BLACK);
	DrawRoundedBox(&sprite, DMCoordinates(0, 0, _headerW, HEADER_H, 0, 0), HEADER_H / 2, GlobalTheme);

	int deviceNameTBWidth = TextBox::DrawTextBox(&sprite,
		TextBoxDto(
			DMCoordinates(
				DEVICE_NAME_TB_X,
				HEADER_TB_Y,
				-1,
				MEDIUM_FONT_TEXT_BOX_H,
				DEVICE_NAME_TB_X + HEADER_X,
				HEADER_TB_Y + HEADER_Y),
			GlobalTheme,
			MEDIUM_FONT,
			MC_DATUM,
			false,
			true,
			GlobalTheme.panelLightColor),
		__DEVICE_NAME__);

	sprite.setFreeFont(MEDIUM_FONT);
	sprite.setTextColor(TFT_GREEN, GlobalTheme.panelLightColor);
	sprite.setTextDatum(ML_DATUM);
	int profileLableX = DEVICE_NAME_TB_X + deviceNameTBWidth + PROFILE_LABEL_X_OFFSET;
	sprite.drawString(PROFILE_LABEL, profileLableX, (HEADER_H /2) - 1);

	int profileX = profileLableX + sprite.textWidth(PROFILE_LABEL);
	TextBox::DrawTextBox(&sprite,
		TextBoxDto(
			DMCoordinates(
				profileX,
				HEADER_TB_Y,
				-1,
				MEDIUM_FONT_TEXT_BOX_H,
				DEVICE_NAME_TB_X + HEADER_X,
				HEADER_TB_Y + HEADER_Y),
			GlobalTheme,
			MEDIUM_FONT,
			MC_DATUM,
			false,
			true,
			GlobalTheme.panelLightColor),
		_currentProfile.name.c_str());

	_tft->pushImageDMA(HEADER_X, HEADER_Y, _headerW, HEADER_H, sprPtr);
	_tft->dmaWait();
	sprite.deleteSprite();
}

void MainScreen::DisplayTime()
{
	_timeSprite->fillSprite(TFT_BLACK);
	_timeSprite->drawString(DateTime.format(DateFormatter::TIME_ONLY), TIME_W, 2);
	_timeSprite->pushSprite(_timeX, TIME_Y);
}