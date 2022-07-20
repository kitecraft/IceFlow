#include "OTAScreen.h"

OTAScreen::OTAScreen(TFT_eSPI* tft)
{
	_tft = tft;
	_tft->fillScreen(TFT_GREEN);
}

OTAScreen::~OTAScreen()
{

}


void OTAScreen::UpdateScreen(int inKey, char* value)
{

}

void OTAScreen::UpdateScreenOnInterval()
{

}


void OTAScreen::HandleTouch(int x, int y)
{

}