#include "Button.h"

Button::Button()
{

}

Button::~Button()
{
	if (_sprite != nullptr) {
		_sprite->deleteSprite();
		delete _sprite;
	}
}

Button::Button(ButtonDto config, String label, TFT_eSPI* tft, bool useSprite)
{
	_config = config;
	_buttonColor = _config.buttonColor;
	_label = label;
	_tft = tft;

	if (useSprite) {
		_sprite = new TFT_eSprite(_tft);
		_sprPtr = (uint16_t*)_sprite->createSprite(_config.coordinates.w, _config.coordinates.h);
	}
}

void Button::Draw()
{
	if (_sprite == nullptr || !_visible) {
		return;
	}

	_sprite->fillSprite(_config.backgroundColor);
	Draw(_sprite);
	_tft->pushImageDMA(_config.coordinates.p_x, _config.coordinates.p_y, _config.coordinates.w, _config.coordinates.h, _sprPtr);
}

void Button::Draw(TFT_eSprite* sprite)
{
	if (_visible) {
		int radius = 4;
		sprite->fillSmoothRoundRect(
			_config.coordinates.x,
			_config.coordinates.y,
			_config.coordinates.w,
			_config.coordinates.h,
			radius,
			_config.theme.panelBorderColor,
			_config.backgroundColor
		);

		sprite->fillSmoothRoundRect(
			_config.coordinates.x + 1,
			_config.coordinates.y + 1,
			_config.coordinates.w - 2,
			_config.coordinates.h - 2,
			radius,
			_config.theme.panelBorderColor,
			_config.theme.panelBorderColor
		);

		sprite->fillSmoothRoundRect(
			_config.coordinates.x + 2,
			_config.coordinates.y + 3,
			_config.coordinates.w - 4,
			_config.coordinates.h - 4,
			radius,
			_buttonColor,
			_config.theme.panelBorderColor
		);

		sprite->setFreeFont(_config.font);
		sprite->setTextColor(_config.theme.textColor);
		sprite->setTextDatum(MC_DATUM);
		sprite->drawString(_label, _config.coordinates.x + _config.coordinates.w / 2, _config.coordinates.y + _config.coordinates.h / 2);
	}

}

bool Button::Touched(int x, int y)
{
	if (!_visible) {
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
