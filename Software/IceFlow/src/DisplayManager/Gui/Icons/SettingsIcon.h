#pragma once
#include "IconBase.h"

#define SETTINGS_ICON_X_OFFSET (9 + ICON_BASE_SPRITE_X_OFFSET)
#define SETTINGS_ICON_HEIGHT 7
#define SETTINGS_ICON_RADIUS 2
#define SETTINGS_ICON_SHORTWIDTH 14
#define SETTINGS_ICON_LONGWIDTH 26
#define SETTINGS_ICON_GAP 4
#define SETTINGS_ICON_COLOR 0xC73B

class SettingsIcon : public IconBase
{
public:
	void Draw(TFT_eSprite* sprite) {
		DrawBase(sprite);

		sprite->fillSmoothRoundRect(coordinates.x + SETTINGS_ICON_X_OFFSET, 10 + coordinates.y, SETTINGS_ICON_SHORTWIDTH, SETTINGS_ICON_HEIGHT, SETTINGS_ICON_RADIUS, SETTINGS_ICON_COLOR);
		sprite->fillSmoothRoundRect(coordinates.x + SETTINGS_ICON_X_OFFSET, 20 + coordinates.y, SETTINGS_ICON_SHORTWIDTH, SETTINGS_ICON_HEIGHT, SETTINGS_ICON_RADIUS, SETTINGS_ICON_COLOR);
		sprite->fillSmoothRoundRect(coordinates.x + SETTINGS_ICON_X_OFFSET, 30 + coordinates.y, SETTINGS_ICON_SHORTWIDTH, SETTINGS_ICON_HEIGHT, SETTINGS_ICON_RADIUS, SETTINGS_ICON_COLOR);

		sprite->fillSmoothRoundRect(coordinates.x + SETTINGS_ICON_X_OFFSET + SETTINGS_ICON_SHORTWIDTH + SETTINGS_ICON_GAP, 10 + coordinates.y, SETTINGS_ICON_LONGWIDTH, SETTINGS_ICON_HEIGHT, SETTINGS_ICON_RADIUS, SETTINGS_ICON_COLOR);
		sprite->fillSmoothRoundRect(coordinates.x + SETTINGS_ICON_X_OFFSET + SETTINGS_ICON_SHORTWIDTH + SETTINGS_ICON_GAP, 20 + coordinates.y, SETTINGS_ICON_LONGWIDTH, SETTINGS_ICON_HEIGHT, SETTINGS_ICON_RADIUS, SETTINGS_ICON_COLOR);
		sprite->fillSmoothRoundRect(coordinates.x + SETTINGS_ICON_X_OFFSET + SETTINGS_ICON_SHORTWIDTH + SETTINGS_ICON_GAP, 30 + coordinates.y, SETTINGS_ICON_LONGWIDTH, SETTINGS_ICON_HEIGHT, SETTINGS_ICON_RADIUS, SETTINGS_ICON_COLOR);
	}
};

