#pragma once
#include <Arduino.h>
#include "Screens_Config.h"

enum THEME_SHADE {
	LIGHT_THEME = 0,
	DARK_THEME
};


class DMTheme
{
public:
	uint16_t panelDarkColor;
	uint16_t panelLightColor;
	uint16_t panelBorderColor;
	uint16_t panelDropShadowColor;
	uint16_t textColor;

	DMTheme() {
		panelDarkColor = -1;
		panelLightColor = -1;
		panelBorderColor = -1;
		panelDropShadowColor = -1;
		textColor = -1;
	}
	DMTheme(uint16_t headerColor, uint16_t backGroundColor, uint16_t borderColor, uint16_t dropShadowColor, uint16_t inTextColor) {
		panelDarkColor = headerColor;
		panelLightColor = backGroundColor;
		panelBorderColor = borderColor;
		panelDropShadowColor = dropShadowColor;
		textColor = inTextColor;
	}
	DMTheme(const DMTheme& orig) {
		panelDarkColor = orig.panelDarkColor;
		panelLightColor = orig.panelLightColor;
		panelBorderColor = orig.panelBorderColor;
		panelDropShadowColor = orig.panelDropShadowColor;
		textColor = orig.textColor;
	}
};


static DMTheme GlobalTheme(
	DM_THEME_PANEL_DARK_COLOR,
	DM_THEME_PANEL_LIGHT_COLOR,
	DM_THEME_BOX_BORDER_COLOR,
	DM_THEME_BOX_DROP_SHADOW,
	DM_THEME_TEXT_COLOR
);