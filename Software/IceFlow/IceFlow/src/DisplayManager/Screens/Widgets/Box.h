#pragma once
#include <TFT_eSPI.h>
#include "../Utilities/DMCoordinates.h"
#include "../Utilities/DMTheme.h"


//Draw a Squared Box using the specified theme
//Creates a 3 pixel wide border including an drop-shadaw
static void DrawSquaredBox(TFT_eSprite* sprite, DMCoordinates coordinates, DMTheme theme, bool useDark = false)
{
	if (useDark) {
		sprite->fillRect(coordinates.x, coordinates.y, coordinates.w, coordinates.h, theme.panelDarkColor);
	}
	else {
		sprite->fillRect(coordinates.x, coordinates.y, coordinates.w, coordinates.h, theme.panelLightColor);
	}
	
	sprite->drawRect(coordinates.x, coordinates.y, coordinates.w, coordinates.h, theme.panelDropShadowColor);
	sprite->drawRect(coordinates.x + 2, coordinates.y + 2, coordinates.w - 3, coordinates.h - 4, theme.panelDropShadowColor);

	sprite->drawRect(coordinates.x, coordinates.y, coordinates.w - 1, coordinates.h - 1, theme.panelBorderColor);
	sprite->drawRect(coordinates.x + 1, coordinates.y + 1, coordinates.w - 3, coordinates.h - 3, theme.panelBorderColor);
}


//Draw a Rounded Box using the specified theme
//Creates a 3 pixel wide border including an drop-shadaw
static void DrawRoundedBox(TFT_eSprite* sprite, DMCoordinates coordinates, int radius, DMTheme theme, bool useDark = false)
{
	sprite->fillSmoothRoundRect(coordinates.x, coordinates.y, coordinates.w, coordinates.h, radius, theme.panelLightColor);

	sprite->drawRoundRect(coordinates.x, coordinates.y, coordinates.w, coordinates.h, radius, theme.panelDropShadowColor);
	sprite->drawRoundRect(coordinates.x + 2, coordinates.y + 2, coordinates.w - 3, coordinates.h - 4, radius, theme.panelDropShadowColor);

	sprite->drawRoundRect(coordinates.x, coordinates.y, coordinates.w - 1, coordinates.h - 1, radius, theme.panelBorderColor);
	sprite->drawRoundRect(coordinates.x + 1, coordinates.y + 1, coordinates.w - 3, coordinates.h - 3, radius, theme.panelBorderColor);
}
