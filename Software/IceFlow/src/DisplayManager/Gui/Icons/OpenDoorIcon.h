#pragma once
#include "IconBase.h"

class OpenDoorIcon : public IconBase
{
public:
	void Draw(TFT_eSprite* sprite) {
		DrawBase(sprite);
		sprite->loadFont(AA_FONT_18PT);
		sprite->setTextColor(ICON_BASE_TEXT_COLOR);
		sprite->setTextDatum(CC_DATUM);
		sprite->drawString("Door", coordinates.x + ICON_BASE_SPRITE_X_OFFSET + coordinates.w / 2, coordinates.y + (coordinates.h / 2));
		sprite->unloadFont();
	}
};

