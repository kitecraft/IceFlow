#pragma once
#include "IconBase.h"

class ProfilesIcon : public IconBase
{
public:
	void Draw(TFT_eSprite* sprite) {
		DrawBase(sprite);
		sprite->loadFont(AA_FONT_14PT);
		sprite->setTextColor(ICON_BASE_TEXT_COLOR);
		sprite->setTextDatum(CC_DATUM);
		sprite->drawString("Profiles", coordinates.x + ICON_BASE_SPRITE_X_OFFSET + coordinates.w / 2, coordinates.y + (coordinates.h / 2));
		sprite->unloadFont();
	}
};

