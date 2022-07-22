#include "IconBase.h"

IconBase::IconBase()
{

}

IconBase::IconBase(IconBaseDto config, TFT_eSPI* tft)
{
	_config = config;
	_tft = tft;
}

bool IconBase::Touched(int x, int y)
{
	if (x >= _config.coordinates.p_x &&
		x <= _config.coordinates.p_x + _config.coordinates.w &&
		y >= _config.coordinates.p_y &&
		y <= _config.coordinates.p_y + _config.coordinates.h)
	{
		return true;
	}
	return false;
}

void IconBase::Draw(TFT_eSprite* sprite)
{
	sprite->fillSmoothRoundRect(_config.coordinates.x, _config.coordinates.y, _config.coordinates.w, _config.coordinates.h, ICON_BASE_ICON_RADIUS,  _config.theme.panelDarkColor);
	sprite->fillSmoothRoundRect(
		_config.coordinates.x + 2,
		_config.coordinates.y + 2,
		_config.coordinates.w - 4,
		_config.coordinates.h - 4,
		ICON_BASE_ICON_RADIUS,
		_config.theme.panelLightColor);
}