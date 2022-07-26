#include "Button.h"

Button::Button()
{

}

Button::~Button()
{

}

Button::Button(ButtonDto config, String label)
{
	_config = config;
	_label = label;
}

void Button::Draw(TFT_eSPI *tft)
{
	Serial.println("Button Draw 1");
	_config.coordinates.Print();
	int radius = 4;
	TFT_eSprite sprite(tft);
	uint16_t* sprPtr = (uint16_t*)sprite.createSprite(_config.coordinates.w, _config.coordinates.h);

	sprite.fillSmoothRoundRect(
		_config.coordinates.x,
		_config.coordinates.y,
		_config.coordinates.w,
		_config.coordinates.h,
		radius,
		_config.theme.panelBorderColor, 
		_config.theme.panelLightColor
	);

	sprite.fillSmoothRoundRect(
		_config.coordinates.x + 1,
		_config.coordinates.y + 1,
		_config.coordinates.w - 2,
		_config.coordinates.h - 2,
		radius,
		_config.theme.panelBorderColor,
		_config.theme.panelBorderColor
	);

	sprite.fillSmoothRoundRect(
		_config.coordinates.x + 2,
		_config.coordinates.y + 3,
		_config.coordinates.w - 4,
		_config.coordinates.h - 4,
		radius,
		_config.buttonColor,
		_config.theme.panelBorderColor
	);

	sprite.setFreeFont(_config.font);
	sprite.setTextColor(_config.theme.textColor, _config.buttonColor);
	sprite.setTextDatum(MC_DATUM);
	sprite.drawString(_label, _config.coordinates.w/2, _config.coordinates.h/2);

	tft->pushImageDMA(_config.coordinates.p_x, _config.coordinates.p_y, _config.coordinates.w, _config.coordinates.h, sprPtr);
	tft->dmaWait();
	sprite.deleteSprite();
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
