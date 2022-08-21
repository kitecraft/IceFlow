#include "StopIcon.h"

StopIcon::StopIcon()
	: IconBase()
{

}

StopIcon::StopIcon(IconBaseDto config, TFT_eSPI* tft)
	: IconBase(config, tft)
{

}

void StopIcon::Draw(TFT_eSprite* sprite)
{
	IconBase::Draw(sprite);

	int r = (_config.coordinates.w - 16) / 2;
	sprite->fillCircle(_config.coordinates.x + (_config.coordinates.w / 2), _config.coordinates.y + (_config.coordinates.h / 2), r, TFT_RED);

	sprite->setFreeFont(SMALL_FONT);
	sprite->setTextColor(TFT_BLACK);
	sprite->setTextDatum(MC_DATUM);
	sprite->drawString("Stop", _config.coordinates.x + (_config.coordinates.w / 2), _config.coordinates.y + (_config.coordinates.h / 2));

	if (!_enabled) {
		UpdateAsDisabled(sprite);
	}
}