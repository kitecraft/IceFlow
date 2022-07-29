#include "Button.h"

Button::Button()
{

}

Button::~Button()
{
	if (_sprite != nullptr) {
		delete _sprite;
	}
}

Button::Button(ButtonDto config, String label, TFT_eSPI* tft)
{
	_config = config;
	_label = label;
	_tft = tft;
	_sprite = new TFT_eSprite(_tft);
	_sprPtr = (uint16_t*)_sprite->createSprite(_config.coordinates.w, _config.coordinates.h);
}

void Button::Draw()
{
	int radius = 4;
	_sprite->fillSmoothRoundRect(
		_config.coordinates.x,
		_config.coordinates.y,
		_config.coordinates.w,
		_config.coordinates.h,
		radius,
		_config.theme.panelBorderColor, 
		_config.theme.panelLightColor
	);

	_sprite->fillSmoothRoundRect(
		_config.coordinates.x + 1,
		_config.coordinates.y + 1,
		_config.coordinates.w - 2,
		_config.coordinates.h - 2,
		radius,
		_config.theme.panelBorderColor,
		_config.theme.panelBorderColor
	);

	_sprite->fillSmoothRoundRect(
		_config.coordinates.x + 2,
		_config.coordinates.y + 3,
		_config.coordinates.w - 4,
		_config.coordinates.h - 4,
		radius,
		_config.buttonColor,
		_config.theme.panelBorderColor
	);

	_sprite->setFreeFont(_config.font);
	_sprite->setTextColor(_config.theme.textColor, _config.buttonColor);
	_sprite->setTextDatum(MC_DATUM);
	_sprite->drawString(_label, _config.coordinates.w/2, _config.coordinates.h/2);

	_tft->pushImageDMA(_config.coordinates.p_x, _config.coordinates.p_y, _config.coordinates.w, _config.coordinates.h, _sprPtr);
	_tft->dmaWait();
}

bool Button::Touched(int x, int y)
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
