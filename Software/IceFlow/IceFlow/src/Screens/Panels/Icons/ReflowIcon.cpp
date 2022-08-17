#include "ReflowIcon.h"


ReflowIcon::ReflowIcon()
	: IconBase()
{

}

ReflowIcon::ReflowIcon(IconBaseDto config, TFT_eSPI* tft)
	: IconBase(config, tft)
{

}

void ReflowIcon::Draw(TFT_eSprite* sprite)
{
	IconBase::Draw(sprite);


	sprite->fillSmoothRoundRect(_config.coordinates.x + 4, _config.coordinates.y + 4, _config.coordinates.w - 8, _config.coordinates.h - 8, 5, _config.theme.panelDarkColor);
	
	sprite->drawFastVLine(_config.coordinates.x + 8, _config.coordinates.y + 7, _config.coordinates.h - 14, _config.theme.panelLightColor);
	sprite->drawFastVLine(_config.coordinates.x + 9, _config.coordinates.y + 7, _config.coordinates.h - 14, _config.theme.panelLightColor);
	sprite->drawFastVLine(_config.coordinates.x + 10, _config.coordinates.y + 7, _config.coordinates.h - 14, _config.theme.panelLightColor);

	int x = _config.coordinates.x + 9;
	int y = _config.coordinates.y + 16;
	sprite->drawCircleHelper(x, y, 8, 0x2, _config.theme.panelLightColor);
	sprite->drawCircleHelper(x, y, 9, 0x2, _config.theme.panelLightColor);
	sprite->drawCircleHelper(x, y, 10, 0x2, _config.theme.panelLightColor);
	sprite->drawCircleHelper(x, y - 2, 8, 0x4, _config.theme.panelLightColor);
	sprite->drawCircleHelper(x, y - 2, 9, 0x4, _config.theme.panelLightColor);
	sprite->drawCircleHelper(x, y - 2, 10, 0x4, _config.theme.panelLightColor);


	y = _config.coordinates.y + 20;
	int h = _config.coordinates.y + _config.coordinates.h - 8;
	sprite->drawLine(x + 5,
		y,
		x + 11,
		h,
		_config.theme.panelLightColor);

	sprite->drawLine(x + 4,
		y + 1,
		x + 10,
		h,
		_config.theme.panelLightColor);

	sprite->drawLine(x + 3,
		y + 2,
		x + 9,
		h,
		_config.theme.panelLightColor);

	x = x + 15;
	y = _config.coordinates.y + 8;
	sprite->drawFastVLine(x, y, _config.coordinates.h - 18, _config.theme.panelLightColor);
	sprite->drawFastVLine(x + 1, y, _config.coordinates.h - 15, _config.theme.panelLightColor);
	sprite->drawFastVLine(x + 2, y, _config.coordinates.h - 158, _config.theme.panelLightColor);

	sprite->drawFastHLine(x + 2, y, 13, _config.theme.panelLightColor);
	sprite->drawFastHLine(x + 2, y + 1, 13, _config.theme.panelLightColor);
	sprite->drawFastHLine(x + 2, y + 2, 13, _config.theme.panelLightColor);

	sprite->drawFastHLine(x + 2, y + 11, 10, _config.theme.panelLightColor);
	sprite->drawFastHLine(x + 2, y + 12, 10, _config.theme.panelLightColor);
	sprite->drawFastHLine(x + 2, y + 13, 10, _config.theme.panelLightColor);

	if (!_enabled) {
		UpdateAsDisabled(sprite);
	}
}