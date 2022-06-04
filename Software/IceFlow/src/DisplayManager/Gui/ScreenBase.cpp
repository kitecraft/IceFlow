#include "ScreenBase.h"

ScreenBase::ScreenBase(TFT_eSPI* newTFT)
{
	TFT = newTFT;
	localError.Clear();
	SPIFFS_Load_Fonts();
}

ScreenBase::~ScreenBase()
{
	TFT = NULL;
	SPIFFS.end();
}

bool ScreenBase::SPIFFS_Load_Fonts()
{
	localError.Clear();
	// ESP32 will crash if any of the fonts are missing
	bool font_is_missing = false;

	if (!SPIFFS.begin()) {
		localError.Set(ERROR_SPIFFS_FAILED, "ScreenBase::HS_Load_Fonts() - Failed to start SPIFFS.");
		return false;
	}

	if (SPIFFS.exists("/SegoeUI-14.vlw") == false) font_is_missing = true;
	if (SPIFFS.exists("/SegoeUI-18.vlw") == false) font_is_missing = true;
	if (SPIFFS.exists("/SegoeUI-20.vlw") == false) font_is_missing = true;

	if (font_is_missing)
	{
		localError.Set(ERROR_FONT_MISSING, "ScreenBase::HS_Load_Fonts() - Failed to find all fonts");
		return false;
	}

	return true;
}

void ScreenBase::DrawBoxWithBorderAndDropShadow(StarsideCoordinates coords, StarsideTheme theme)
{
	TFT->fillRect(coords.x, coords.y, coords.w, coords.h, theme.panelBGColor);
	DrawBorderandDropShadow(coords, theme);
}

void ScreenBase::DrawBorderandDropShadow(StarsideCoordinates coords, StarsideTheme theme)
{
	TFT->drawRect(coords.x, coords.y, coords.w, coords.h, theme.panelDropShadowColor);
	TFT->drawRect(coords.x + 2, coords.y + 2, coords.w - 3, coords.h - 4, theme.panelDropShadowColor);

	TFT->drawRect(coords.x, coords.y, coords.w - 1, coords.h - 1, theme.panelBorderColor);
	TFT->drawRect(coords.x + 1, coords.y + 1, coords.w - 3, coords.h - 3, theme.panelBorderColor);
}

void ScreenBase::DrawBoxWithBorderAndDropShadow(TFT_eSprite *sprite, StarsideCoordinates coords, StarsideTheme theme)
{

	sprite->fillRect(coords.x, coords.y, coords.w, coords.h, theme.panelBGColor);

	DrawBorderandDropShadow(sprite, coords, theme);
}

void ScreenBase::DrawBorderandDropShadow(TFT_eSprite *sprite, StarsideCoordinates coords, StarsideTheme theme)
{
	sprite->drawRect(coords.x, coords.y, coords.w, coords.h, theme.panelDropShadowColor);
	sprite->drawRect(coords.x + 2, coords.y + 2, coords.w - 3, coords.h - 4, theme.panelDropShadowColor);

	sprite->drawRect(coords.x, coords.y, coords.w - 1, coords.h - 1, theme.panelBorderColor);
	sprite->drawRect(coords.x + 1, coords.y + 1, coords.w - 3, coords.h - 3, theme.panelBorderColor);
}

void ScreenBase::DrawRoundedBox(TFT_eSprite* sprite, StarsideCoordinates coordinates, StarsideTheme theme, int radius, int border)
{

	sprite->fillSmoothRoundRect(coordinates.x, coordinates.y, coordinates.w, coordinates.h, radius, theme.panelBorderColor, TFT_TRANSPARENT);
	sprite->fillSmoothRoundRect(
		coordinates.x + border,
		coordinates.y + border,
		coordinates.w - (border * 2),
		coordinates.h - (border * 2),
		radius,
		theme.panelHeaderColor,
		TFT_TRANSPARENT);

	/*
	sprite->fillRoundRect(coordinates.x, coordinates.y, coordinates.w, coordinates.h, radius, theme.panelHeaderColor);

	sprite->drawRoundRect(coordinates.x, coordinates.y, coordinates.w, coordinates.h, radius, theme.panelDropShadowColor);
	sprite->drawRoundRect(coordinates.x + 2, coordinates.y + 2, coordinates.w - 3, coordinates.h - 4, radius, theme.panelDropShadowColor);

	sprite->drawRoundRect(coordinates.x, coordinates.y, coordinates.w - 1, coordinates.h - 1, radius, theme.panelBorderColor);
	sprite->drawRoundRect(coordinates.x + 1, coordinates.y + 1, coordinates.w - 3, coordinates.h - 3, radius, theme.panelBorderColor);
	*/
}
/*

void ScreenBase::DrawBoxWithBorderDropShadowAndText(TFT_eSprite *sprite, StarsideCoordinates coords, StarsideTheme theme)
{
	DrawBorderandDropShadow(sprite, coords, theme);
	//text here
}



void ScreenBase::DrawPillBoxWithDropShadow(TFT_eSprite *sprite, StarsideCoordinates coordinates, StarsideTheme theme)
{
	int textAreaWidth = (coordinates.w + (TEXT_HORIZONTAL_BUFFER * 2));
	int textAreaHeight = coordinates.h + (TEXT_VERTICAL_BUFFER * 2);

	sprite->fillRoundRect(coordinates.x, coordinates.y, textAreaWidth, textAreaHeight, textAreaHeight / 2, theme.panelBGColor);

	sprite->drawRoundRect(coordinates.x, coordinates.y, textAreaWidth, textAreaHeight, textAreaHeight / 2, theme.panelDropShadowColor);
	sprite->drawRoundRect(coordinates.x + 2, coordinates.y + 2, textAreaWidth - 3, textAreaHeight - 4, textAreaHeight / 2, theme.panelDropShadowColor);

	sprite->drawRoundRect(coordinates.x, coordinates.y, textAreaWidth - 1, textAreaHeight - 1, textAreaHeight / 2, theme.panelBorderColor);
	sprite->drawRoundRect(coordinates.x + 1, coordinates.y + 1, textAreaWidth - 3, textAreaHeight - 3, textAreaHeight / 2, theme.panelBorderColor);
}

*/

bool ScreenBase::CheckForErrorOnLast()
{
	if(localError.code == NO_ERROR){
		return false;
	}
	return true;
}

StarsideError ScreenBase::GetLastError()
{
	return localError;
}

void ScreenBase::DisplayFreeHeap()
{
	Serial.println(ESP.getFreeHeap());
}