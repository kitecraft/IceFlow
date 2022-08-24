#include "TemplateScreen.h"

TemplateScreen::TemplateScreen(TFT_eSPI* tft)
{
	_tft = tft;
	_tft->fillScreen(TFT_GREEN);
}

TemplateScreen::~TemplateScreen()
{

}


void TemplateScreen::UpdateScreen(int inKey, char* value)
{

}

void TemplateScreen::UpdateScreenOnInterval()
{

}


void TemplateScreen::HandleTouch(int x, int y)
{
	_tft->startWrite();
	ProcessTouch(x, y);
	_tft->dmaWait();
	_tft->endWrite();
}

void TemplateScreen::ProcessTouch(int x, int y)
{

}

void TemplateScreen::DrawScreen()
{

}