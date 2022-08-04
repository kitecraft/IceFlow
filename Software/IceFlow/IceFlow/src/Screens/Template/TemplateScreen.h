#pragma once
#include <Arduino.h>
#include <TFT_eSPI.h>


class TemplateScreen
{
private:
	TFT_eSPI* _tft;

public:
	TemplateScreen(TFT_eSPI* tft);
	~TemplateScreen();

	void UpdateScreen(int inKey, char* value);
	void UpdateScreenOnInterval();

	void HandleTouch(int x, int y);

};


static TemplateScreen* templateScreen = nullptr;
static void Create_TemplateScreen(TFT_eSPI* TFT) 
{
	templateScreen = new TemplateScreen(TFT);
}

static void Destroy_TemplateScreen()
{
	if (templateScreen != nullptr)
	{
		delete(templateScreen);
		templateScreen = nullptr;
	}
}

static void Update_TemplateScreen(int inKey, char* value)
{
	templateScreen->UpdateScreen(inKey, value);
}

static void Update_TemplateScreen_OnInterval()
{
	templateScreen->UpdateScreenOnInterval();
}

static void HandleTouch_TemplateScreen(int x, int y)
{
	templateScreen->HandleTouch(x, y);
}


