#include "MainScreen.h"
#include "Utilities/ScreenUpdateKeys.h"
#include "Widgets/WiFiSymbol.h"

MainScreen::MainScreen(TFT_eSPI* tft)
{
	_tft = tft;
	_wifiX = _tft->width() - WIFI_SYMBOL_WIDTH;

	DrawScreen();
}

MainScreen::~MainScreen()
{

}


void MainScreen::UpdateScreen(int inKey, char* value)
{
	_tft->startWrite();
	SCREEN_UPDATE_KEYS key = static_cast<SCREEN_UPDATE_KEYS>(inKey);
	switch (key) {
	case suk_Network_Connected:
		Serial.println("Updated wifi STA connected");
		WiFiSymbol::Draw(_wifiX, WIFI_Y, WiFiStatus_STA, TFT_BLACK, _tft);
		break;
	case suk_Network_Started:
		Serial.println("Updated wifi AP started");
		WiFiSymbol::Draw(_wifiX, WIFI_Y, WiFiStatus_AP, TFT_BLACK, _tft);
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

}

void MainScreen::DrawScreen()
{
	_tft->fillScreen(TFT_BLACK);
	_tft->startWrite();

	WiFiSymbol::Draw(_wifiX, WIFI_Y, WiFiStatus_Off, TFT_BLACK, _tft);

	_tft->dmaWait();
	_tft->endWrite();
}