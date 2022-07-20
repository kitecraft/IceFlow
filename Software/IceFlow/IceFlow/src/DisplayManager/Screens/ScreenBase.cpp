#include "ScreenBase.h"
#include "Utilities/LoadFonts.h"

ScreenBase::ScreenBase(TFT_eSPI* tft)
{
	_tft = tft;
	if (!LoadFonts()) {
		Serial.println("Failed to load fonts");
		_tft->fillScreen(TFT_RED);
		_tft->setCursor(20, 20);
		_tft->setTextSize(8);
		_tft->setTextColor(TFT_BLACK);
		_tft->print("Failed to load fonts");
		while (true) { vTaskDelay(1); }
	}
}