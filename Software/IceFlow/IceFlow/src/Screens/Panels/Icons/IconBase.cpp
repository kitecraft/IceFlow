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
	if (!_enabled) {
		return false;
	}

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

void IconBase::UpdateAsDisabled(TFT_eSprite* sprite)
{
	for (int row = 0; row < _config.coordinates.h; row++) {
		for (int column = 0; column < _config.coordinates.w ; column++) {
			int x = column + _config.coordinates.x;
			int y = row + _config.coordinates.y;
			uint16_t color = sprite->readPixel(x, y);
			uint16_t newColor = _tft->alphaBlend(DISABLED_BLEND_RATIO, color, DISABLED_BLEND_COLOR);
			sprite->drawPixel(x, y, newColor);
		}
	}
}