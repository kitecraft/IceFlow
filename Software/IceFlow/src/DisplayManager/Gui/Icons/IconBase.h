#pragma once
#include <Arduino.h>
#include <TFT_eSPI.h>
#include "../../../IceFlow_Config.h"
#include "../../Utilities/StarsideCoordinates.h"

#define ICON_BASE_SPRITE_X_OFFSET 7
#define ICON_BASE_BACKGROUND_COLOR 0xCE1B
#define ICON_BASE_BORDER_WIDTH 2
#define ICON_BASE_ICON_RADIUS 6
#define ICON_BASE_ICON_COLOR 0x738E
#define ICON_BASE_TEXT_COLOR GLOBAL_TEXT_COLOR //0xF360

class IconBase
{
public:
	TFT_eSPI* TFT;
	StarsideCoordinates coordinates;

	void Init(TFT_eSPI* inTFT, StarsideCoordinates inCoordinates);
	bool Touched(int x, int y);
	void DrawBase(TFT_eSprite* sprite) {
		sprite->fillSmoothRoundRect(ICON_BASE_SPRITE_X_OFFSET, coordinates.y, coordinates.w, coordinates.h, ICON_BASE_ICON_RADIUS, ICON_BASE_BACKGROUND_COLOR);
		sprite->fillSmoothRoundRect(
			ICON_BASE_SPRITE_X_OFFSET + ICON_BASE_BORDER_WIDTH,
			coordinates.y + ICON_BASE_BORDER_WIDTH,
			coordinates.w - (ICON_BASE_BORDER_WIDTH * 2),
			coordinates.h - (ICON_BASE_BORDER_WIDTH * 2),
			ICON_BASE_ICON_RADIUS,
			ICON_BASE_ICON_COLOR);
	}
};

