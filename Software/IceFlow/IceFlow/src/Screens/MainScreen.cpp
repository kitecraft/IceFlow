#include "MainScreen.h"
#include <ESPDateTime.h>
#include "Utilities/ScreenUpdateKeys.h"
#include "Utilities/DMTheme.h"

MainScreen::MainScreen(TFT_eSPI* tft)
{
	_tft = tft;
	_wifiX = _tft->width() - 4;
	_timeX = _wifiX - TIME_W - 7;
	_sidebarX = _tft->width() - SIDEBAR_W;
	_sideBar = new SideBar(_tft,DMCoordinates(_sidebarX, SIDEBAR_Y, SIDEBAR_W, SIDEBAR_H, _sidebarX, SIDEBAR_Y));

	DrawScreen();
}

MainScreen::~MainScreen()
{
	delete(_sideBar);
}


void MainScreen::UpdateScreen(int inKey, char* value)
{
	_tft->startWrite();
	SCREEN_UPDATE_KEYS key = static_cast<SCREEN_UPDATE_KEYS>(inKey);
	switch (key) {
	case suk_Network_Connected:
		_tft->drawSpot(_wifiX, WIFI_Y, WIFI_SPOT_R, TFT_GREEN, TFT_BLACK);
		break;
	case suk_Network_Started:
		_tft->drawSpot(_wifiX, WIFI_Y, WIFI_SPOT_R, TFT_YELLOW, TFT_BLACK);
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
	//Serial.println("MainScreen Handling Touch");
	if(_sideBar->Touched(x, y)){
		Serial.println("Sidebar was touched");
	}
}

void MainScreen::DrawScreen()
{
	_tft->fillScreen(TFT_BLACK);
	_tft->startWrite();

	//WiFiSymbol::Draw(_wifiX, WIFI_Y, WiFiStatus_Off, TFT_BLACK, _tft);
	_tft->drawSpot(_wifiX, WIFI_Y, WIFI_SPOT_R, TFT_WHITE, TFT_BLACK);
	_sideBar->Draw();

	_tft->dmaWait();
	_tft->endWrite();
}

void IRAM_ATTR MainScreen::OnTimer()
{

}

void MainScreen::DisplayTime()
{
	TFT_eSprite sprite(_tft);
	uint16_t* sprPtr = (uint16_t*)sprite.createSprite(TIME_W, TIME_H);
	sprite.fillSprite(TFT_BLACK);
	sprite.setFreeFont(SMALL_FONT);
	sprite.setTextColor(GlobalTheme.textColor, TFT_BLACK);
	sprite.setTextDatum(TR_DATUM);
	sprite.drawString(DateTime.format(DateFormatter::TIME_ONLY), TIME_W, 2);


	_tft->pushImageDMA(_timeX, TIME_Y, TIME_W, TIME_H, sprPtr);
	_tft->dmaWait();
	sprite.deleteSprite();
}