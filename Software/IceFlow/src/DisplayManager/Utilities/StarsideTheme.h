#pragma once
#include <Arduino.h>

#define GLOBAL_TEXT_COLOR 0xCE59
#define GLOBAL_PANEL_HCOLOR 0x5AEB
#define GLOBAL_PANEL_BGCOLOR 0x738E
#define GLOBAL_BOX_BORDER_COLOR 0x9CD3
#define GLOBAL_BOX_DROP_SHADOW 0x4208


class StarsideTheme
{
public:
	uint16_t panelHeaderColor;
	uint16_t panelBGColor;
	uint16_t panelBorderColor;
	uint16_t panelDropShadowColor;
	uint16_t textColor;

	StarsideTheme() {
		panelHeaderColor = 0;
		panelBGColor = 0;
		panelBorderColor = 0;
		panelDropShadowColor = 0;
		textColor = 0;
	}
	StarsideTheme(uint16_t headerColor, uint16_t backGroundColor, uint16_t borderColor, uint16_t dropShadowColor, uint16_t inTextColor) {
		panelHeaderColor = headerColor;
		panelBGColor = backGroundColor;
		panelBorderColor = borderColor;
		panelDropShadowColor = dropShadowColor;
		textColor = inTextColor;
	}
	StarsideTheme(const StarsideTheme& orig) {
		panelHeaderColor = orig.panelHeaderColor;
		panelBGColor = orig.panelBGColor;
		panelBorderColor = orig.panelBorderColor;
		panelDropShadowColor = orig.panelDropShadowColor;
		textColor = orig.textColor;
	}
};

static StarsideTheme g_GlobalTheme(GLOBAL_PANEL_HCOLOR, GLOBAL_PANEL_BGCOLOR, GLOBAL_BOX_BORDER_COLOR, GLOBAL_BOX_DROP_SHADOW, GLOBAL_TEXT_COLOR);