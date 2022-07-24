#include "MenuOption.h"
#include "Box.h"

MenuOption::MenuOption()
{
}

MenuOption::MenuOption(String name, PopUpMenuDto config)
{
	_name = name;
	_config = config;
	_config.coordinates.w = (_config.coordinates.w + _config.coordinates.h + 6);
}

MenuOption::MenuOption(const MenuOption& orig)
{
	_name = orig._name;
	_config = orig._config;
}
/*
void MenuOption::DrawPillBoxWithDropShadow(TFT_eSprite* sprite)
{
	
	sprite->fillRoundRect(_config.coordinates.x, _config.coordinates.y, _config.coordinates.w, _config.coordinates.h, _config.coordinates.h / 2, _config.theme.panelDarkColor);

	sprite->drawRoundRect(_config.coordinates.x, _config.coordinates.y, _config.coordinates.w, _config.coordinates.h, _config.coordinates.h / 2, _config.theme.panelDropShadowColor);
	sprite->drawRoundRect(_config.coordinates.x + 2, _config.coordinates.y + 2, _config.coordinates.w - 3, _config.coordinates.h - 4, _config.coordinates.h / 2, _config.theme.panelDropShadowColor);

	sprite->drawRoundRect(_config.coordinates.x, _config.coordinates.y, _config.coordinates.w - 1, _config.coordinates.h - 1, _config.coordinates.h / 2, _config.theme.panelBorderColor);
	sprite->drawRoundRect(_config.coordinates.x + 1, _config.coordinates.y + 1, _config.coordinates.w - 3, _config.coordinates.h - 3, _config.coordinates.h / 2, _config.theme.panelBorderColor);
}
*/
void MenuOption::Draw(TFT_eSprite* sprite)
{
	//DrawPillBoxWithDropShadow(sprite);
	DrawRoundedBox(sprite, _config.coordinates, _config.coordinates.h/2, _config.theme, _config.theme.panelLightColor, true);
	sprite->setTextDatum(MC_DATUM);
	sprite->setTextColor(_config.theme.textColor, _config.theme.panelDarkColor);
	sprite->drawString(_name, _config.coordinates.x + (_config.coordinates.w / 2), (_config.coordinates.y + (_config.coordinates.h / 2)) - 1);
}

bool MenuOption::Touched(int x, int y)
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