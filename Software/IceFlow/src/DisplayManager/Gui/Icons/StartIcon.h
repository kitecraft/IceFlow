#pragma once
#include "IconBase.h"

#define START_ICON_ENABLED_COLOR 0x0E43
#define START_ICON_DISABLED_COLOR 0xF164
#define START_ICON_BACKGROUND_COLOR TFT_BLACK
#define START_ICON_TEXT_COLOR 0xCB61

class StartIcon : public IconBase
{
public:
	void Draw(TFT_eSprite* sprite) {
		DrawBase(sprite);

		sprite->fillSmoothCircle(
			ICON_BASE_SPRITE_X_OFFSET + coordinates.w / 2,
			coordinates.y + coordinates.h / 2,
			(coordinates.h / 2) - 4,
			START_ICON_BACKGROUND_COLOR);
		sprite->fillSmoothCircle(
			ICON_BASE_SPRITE_X_OFFSET + coordinates.w / 2,
			coordinates.y + coordinates.h / 2,
			(coordinates.h / 2) - 5, 
			START_ICON_DISABLED_COLOR);
		
		sprite->drawLine(
			ICON_BASE_SPRITE_X_OFFSET + 4, 
			coordinates.y + 4,
			ICON_BASE_SPRITE_X_OFFSET + coordinates.w - 4,
			coordinates.y + coordinates.h - 4,
			TFT_BLACK);

		sprite->drawLine(
			ICON_BASE_SPRITE_X_OFFSET + 5,
			coordinates.y + 5,
			ICON_BASE_SPRITE_X_OFFSET + coordinates.w - 3,
			coordinates.y + coordinates.h - 3,
			TFT_BLACK);

		sprite->drawLine(
			ICON_BASE_SPRITE_X_OFFSET + coordinates.w - 4,
			coordinates.y + 4,
			ICON_BASE_SPRITE_X_OFFSET + 4,
			coordinates.y + coordinates.h - 4,
			TFT_BLACK);

		sprite->drawLine(
			ICON_BASE_SPRITE_X_OFFSET + coordinates.w - 5,
			coordinates.y + 5,
			ICON_BASE_SPRITE_X_OFFSET + 5,
			coordinates.y + coordinates.h - 5,
			TFT_BLACK);

		sprite->loadFont(AA_FONT_20PT);
		sprite->setTextColor(START_ICON_TEXT_COLOR);
		sprite->setTextDatum(CC_DATUM);
		sprite->drawString("Go", ICON_BASE_SPRITE_X_OFFSET + coordinates.w / 2, 1 + (coordinates.y + coordinates.h / 2));
		sprite->unloadFont();
	}
};

