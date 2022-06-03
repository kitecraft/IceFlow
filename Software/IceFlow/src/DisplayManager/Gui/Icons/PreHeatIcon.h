#pragma once
#include "IconBase.h"

#define TEXT_LINE_Y_OFFSET 4

class PreHeatIcon : public IconBase
{
public:
	void Draw(TFT_eSprite* sprite) {
		enabled = false;
		DrawBase(sprite);
		sprite->loadFont(AA_FONT_18PT);
		sprite->setTextColor(ICON_BASE_TEXT_COLOR);
		sprite->setTextDatum(TC_DATUM);
		sprite->drawString("Pre-", ICON_BASE_SPRITE_X_OFFSET + coordinates.w / 2, TEXT_LINE_Y_OFFSET + coordinates.y);
		sprite->drawString("Heat", ICON_BASE_SPRITE_X_OFFSET + coordinates.w / 2, coordinates.y + 22);
		sprite->unloadFont();
	}
};

