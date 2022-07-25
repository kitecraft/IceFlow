#include "ProfileIcon.h"

ProfileIcon::ProfileIcon()
	: IconBase()
{

}

ProfileIcon::ProfileIcon(IconBaseDto config, TFT_eSPI* tft)
	: IconBase(config, tft)
{

}

void ProfileIcon::Draw(TFT_eSprite* sprite)
{

	IconBase::Draw(sprite);

	sprite->drawLine(
		_config.coordinates.x + 4,
		_config.coordinates.y + _config.coordinates.h - 11,
		_config.coordinates.x + 14,
		_config.coordinates.y + _config.coordinates.h - 25,
		TFT_RED);


	sprite->drawFastHLine(
		_config.coordinates.x + 14,
		_config.coordinates.y + _config.coordinates.h - 25,
		4,
		TFT_RED);

	sprite->drawLine(
		_config.coordinates.x + 18,
		_config.coordinates.y + _config.coordinates.h - 25,
		_config.coordinates.x + 28,
		_config.coordinates.y + _config.coordinates.h - 37,
		TFT_RED);

	sprite->drawFastHLine(
		_config.coordinates.x + 28,
		_config.coordinates.y + _config.coordinates.h - 37,
		8,
		TFT_RED);

	sprite->drawLine(
		_config.coordinates.x + 36,
		_config.coordinates.y + _config.coordinates.h - 37,
		_config.coordinates.x + 43,
		_config.coordinates.y + _config.coordinates.h - 11,
		TFT_RED);
}